/* Decompress a JPEG image, compress the result in several ways, and
 * decompress those in several ways, printing a hash of all the results.
 *
 * The testscript runs this with the SIMD extensions selected for the CPU,
 * restricted, and disabled, and verifies that the hashes are the same (which
 * upstream guarantees for the accurate integer DCT/IDCT used here).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <jpeglib.h>

#undef NDEBUG
#include <assert.h>

struct buffer
{
  unsigned char* data;
  size_t size;
};

static void
append (struct buffer* b, const unsigned char* data, size_t size)
{
  b->data = realloc (b->data, b->size + size);
  assert (b->data != NULL);
  memcpy (b->data + b->size, data, size);
  b->size += size;
}

static struct buffer
read_file (const char* path)
{
  FILE* f = fopen (path, "rb");
  struct buffer r = {NULL, 0};
  unsigned char data[4096];
  size_t n;

  assert (f != NULL);

  while ((n = fread (data, 1, sizeof (data), f)) != 0)
    append (&r, data, n);

  assert (!ferror (f));
  fclose (f);
  return r;
}

/* Decompress the JPEG image using the accurate integer IDCT, appending the
 * result to the buffer. Plain (rather than fancy) upsampling of a 4:2:0
 * image to RGB uses merged upsampling and color conversion.
 */
static void
decompress (const struct buffer* jpeg, J_COLOR_SPACE space, boolean fancy,
            unsigned int scale, struct buffer* out,
            int* width, int* height)
{
  struct jpeg_decompress_struct d;
  struct jpeg_error_mgr e;
  size_t stride;
  unsigned char* row;

  d.err = jpeg_std_error (&e);
  jpeg_create_decompress (&d);
  jpeg_mem_src (&d, jpeg->data, (unsigned long) jpeg->size);
  jpeg_read_header (&d, TRUE);

  d.out_color_space = space;
  d.do_fancy_upsampling = fancy;
  d.dct_method = JDCT_ISLOW;
  d.scale_num = 1;
  d.scale_denom = scale;

  jpeg_start_decompress (&d);

  if (width != NULL)
  {
    *width = (int) d.output_width;
    *height = (int) d.output_height;
  }

  stride = (size_t) d.output_width * d.output_components;
  row = malloc (stride);
  assert (row != NULL);

  while (d.output_scanline < d.output_height)
  {
    jpeg_read_scanlines (&d, &row, 1);
    append (out, row, stride);
  }

  free (row);
  jpeg_finish_decompress (&d);
  jpeg_destroy_decompress (&d);
}

/* Compress the RGB image using the accurate integer DCT and the specified
 * luma sampling factor (2 for 4:2:0, 1 for 4:4:4), optionally progressive.
 */
static struct buffer
compress (const struct buffer* rgb, int width, int height,
          int sampling, boolean progressive)
{
  struct jpeg_compress_struct c;
  struct jpeg_error_mgr e;
  unsigned char* data = NULL;
  unsigned long size = 0;
  struct buffer r;

  c.err = jpeg_std_error (&e);
  jpeg_create_compress (&c);
  jpeg_mem_dest (&c, &data, &size);

  c.image_width = (JDIMENSION) width;
  c.image_height = (JDIMENSION) height;
  c.input_components = 3;
  c.in_color_space = JCS_RGB;

  jpeg_set_defaults (&c);
  jpeg_set_quality (&c, 95, TRUE);
  c.dct_method = JDCT_ISLOW;
  c.comp_info[0].h_samp_factor = sampling;
  c.comp_info[0].v_samp_factor = sampling;

  if (progressive)
    jpeg_simple_progression (&c);

  jpeg_start_compress (&c, TRUE);

  while (c.next_scanline < c.image_height)
  {
    JSAMPROW row = rgb->data + (size_t) c.next_scanline * width * 3;
    jpeg_write_scanlines (&c, &row, 1);
  }

  jpeg_finish_compress (&c);
  jpeg_destroy_compress (&c);

  r.data = data;
  r.size = size;
  return r;
}

/* 64-bit FNV-1a hash.
 */
static uint64_t
hash (const struct buffer* b)
{
  uint64_t h = 0xcbf29ce484222325ULL;
  size_t i;

  for (i = 0; i != b->size; ++i)
  {
    h ^= b->data[i];
    h *= 0x100000001b3ULL;
  }

  return h;
}

int
main (int argc, char* argv[])
{
  struct buffer jpeg;
  struct buffer rgb = {NULL, 0};
  struct buffer r = {NULL, 0};
  int width, height;
  int i;

  assert (argc == 2);
  jpeg = read_file (argv[1]);

  decompress (&jpeg, JCS_RGB, TRUE, 1, &rgb, &width, &height);
  append (&r, rgb.data, rgb.size);

  for (i = 0; i != 3; ++i)
  {
    struct buffer j = compress (&rgb, width, height,
                                i == 1 ? 1 : 2, /* 4:4:4 or 4:2:0. */
                                i == 2);        /* Progressive. */
    append (&r, j.data, j.size);

    decompress (&j, JCS_RGB,      TRUE,  1, &r, NULL, NULL);
    decompress (&j, JCS_RGB,      FALSE, 1, &r, NULL, NULL);
    decompress (&j, JCS_EXT_RGBA, TRUE,  1, &r, NULL, NULL);
    decompress (&j, JCS_YCbCr,    TRUE,  1, &r, NULL, NULL);
    decompress (&j, JCS_RGB,      TRUE,  2, &r, NULL, NULL);
    decompress (&j, JCS_RGB,      TRUE,  4, &r, NULL, NULL);

    free (j.data);
  }

  printf ("%016llx\n", (unsigned long long) hash (&r));

  free (r.data);
  free (rgb.data);
  free (jpeg.data);
  return 0;
}
