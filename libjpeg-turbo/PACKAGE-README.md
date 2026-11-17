# libjpeg-turbo - JPEG image codec C library

This is a `build2` package for the
[`libjpeg-turbo`](https://github.com/libjpeg-turbo/libjpeg-turbo) C library.
It provides a JPEG image codec that uses SIMD instructions to accelerate
baseline and progressive JPEG compression and decompression on systems with
common CPU architectures. It implements the traditional libjpeg API (emulating
the libjpeg v6b API/ABI by default) and features colorspace extensions that allow it to
compress from/decompress to 32-bit and big-endian pixel buffers (RGBX, XBGR,
etc). Note that this package does not provide the TurboJPEG API.


## Usage

To start using `libjpeg-turbo` in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libjpeg-turbo ^3.2.1
```

Then import the library in your `buildfile`:

```
import libs = libjpeg-turbo%lib{jpeg}
```


## Importable targets

This package provides the following importable targets:

```
lib{jpeg}
```

The libjpeg API library. Its public headers are included as `<jpeglib.h>`
(which requires `<stdio.h>` to be included first), `<jerror.h>`,
`<jmorecfg.h>`, and `<jconfig.h>`.


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.libjpeg_turbo.jpeg7 ?= false
[bool] config.libjpeg_turbo.jpeg8 ?= false
[bool] config.libjpeg_turbo.simd  ?= true
```

By default the library emulates the libjpeg v6b API/ABI. The `jpeg7` and
`jpeg8` variables make it emulate the libjpeg v7 or v8 API/ABI instead (if
both are true, then the latest is emulated). Note that either makes the
library backward-incompatible with libjpeg v6b. See the upstream
[README](https://github.com/libjpeg-turbo/libjpeg-turbo#libjpeg-v7-and-v8-apiabi-emulation)
for details.

The `simd` variable controls whether the SIMD extensions are built, which
this package supports on `x86_64`, `i386`, and `aarch64` (on other targets
the library is always built without them). On `x86_64` and `i386` they
require the `nasm` assembler, which is then a build-time dependency of this
package.
