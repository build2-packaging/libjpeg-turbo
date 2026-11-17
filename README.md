# libjpeg-turbo - JPEG image codec C library

This is a `build2` package repository for [`libjpeg-turbo`](https://github.com/libjpeg-turbo/libjpeg-turbo),
a JPEG image codec that uses SIMD instructions to accelerate baseline and
progressive JPEG compression and decompression.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`libjpeg-turbo` in your `build2`-based project, then instead see the accompanying
[`PACKAGE-README.md`](libjpeg-turbo/PACKAGE-README.md) file.

The development setup for `libjpeg-turbo` uses the standard `bdep`-based workflow.
For example:

```
git clone --recursive https://github.com/build2-packaging/libjpeg-turbo.git
cd libjpeg-turbo

bdep init -C @gcc cc config.c=gcc
bdep update
bdep test
```
