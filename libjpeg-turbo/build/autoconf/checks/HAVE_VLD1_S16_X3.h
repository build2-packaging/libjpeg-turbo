// HAVE_VLD1_S16_X3

#undef HAVE_VLD1_S16_X3

/* Check for the vld1_s16_x3() function.
 * vld1_s16_x3() is an Arm Neon intrinsic that loads three vectors of four
 * 16-bit signed integers.
 *
 * Available on AArch64 Linux/glibc, MacOS, and Windows including MinGW with
 * GCC 9, Clang (exact version is unclear but for a while now), and MSVC 19.20
 * (Visual Studio 2019).
 */
#if (defined(__aarch64__) || defined(_M_ARM64))  && \
    (defined(__clang__)                         || \
     (defined(__GNUC__) && __GNUC__ >= 9)       || \
     (defined(_MSC_VER) && _MSC_VER >= 1920))
#  define HAVE_VLD1_S16_X3 1
#endif
