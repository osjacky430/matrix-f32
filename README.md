[![Build Status](https://travis-ci.org/osjacky430/matrix_f32.svg?branch=master)](https://travis-ci.org/osjacky430/matrix_f32)
# Matrix library
A C language matrix library with floating point as storage type, this is originally used in my own embedded system project, accompanied with FreeRTOS for more stable dynamic memory allocation pvPortMalloc instead of malloc.
## Getting started
Basically, just copy the header and source file to your directory. It can also be done by building static library, however, there may be some undefined reference due the compiler linking mechanism and the compile flag, thus the former approach is recommended. If you still want to build static library, then the following instruction will help you with it.
### Prerequisites
The toolchain in use is arm-none-eabi-gcc.
After you downloaded the toolchain, remember to set the binary file folder to PATH, i.e.
```
export PATH="$PATH:/path/to/arm-gcc-toolchain/bin"
```
### Build static library
To build the static library, just create the build file, and run cmake with flag -DBUILD_STATIC_LIBRARY=ON.
```
cd /path/to/matrix_f32
mkdir build && cd build
cmake -DBUILD_STATIC_LIBRARY=ON ..
cmake --build ./
```
### Build tests
Currently the test is done using Visual Studio Test Explorer under google test framework, this part will be added if I manage to conduct unit test using arm-none-eabi-gcc toolchain.
## Built with
[cmake](https://cmake.org/) - Build, and test.
