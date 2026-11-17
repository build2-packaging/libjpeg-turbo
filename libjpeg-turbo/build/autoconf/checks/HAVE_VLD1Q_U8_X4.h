// HAVE_VLD1Q_U8_X4

#undef HAVE_VLD1Q_U8_X4

/* Check for the vld1q_u8_x4() function.
 * vld1q_u8_x4() is an Arm Neon intrinsic that loads four vectors of sixteen
 * 8-bit unsigned integers.
 *
 * Available on AArch64 Linux/glibc, MacOS, and Windows including MinGW with
 * GCC 10, Clang (exact version is unclear but for a while now), and MSVC
 * 19.20 (Visual Studio 2019).
 */
#if (defined(__aarch64__) || defined(_M_ARM64))  && \
    (defined(__clang__)                         || \
     (defined(__GNUC__) && __GNUC__ >= 10)      || \
     (defined(_MSC_VER) && _MSC_VER >= 1920))
#  define HAVE_VLD1Q_U8_X4 1
#endif
