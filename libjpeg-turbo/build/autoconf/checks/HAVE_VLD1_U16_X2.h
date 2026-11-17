// HAVE_VLD1_U16_X2

#undef HAVE_VLD1_U16_X2

/* Check for the vld1_u16_x2() function.
 * vld1_u16_x2() is an Arm Neon intrinsic that loads two vectors of four
 * 16-bit unsigned integers.
 *
 * Available on AArch64 Linux/glibc, MacOS, and Windows including MinGW with
 * GCC 8, Clang (exact version is unclear but for a while now), and MSVC 19.20
 * (Visual Studio 2019).
 */
#if (defined(__aarch64__) || defined(_M_ARM64))  && \
    (defined(__clang__)                         || \
     (defined(__GNUC__) && __GNUC__ >= 8)       || \
     (defined(_MSC_VER) && _MSC_VER >= 1920))
#  define HAVE_VLD1_U16_X2 1
#endif
