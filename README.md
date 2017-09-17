# nixie-clock

An open source nixie clock synchronized to GPS time.

![alt text](/pictures/3d_render.png?raw=true)

## Build Instructions

The code was built on Ubuntu 16.04.  You will need to install the [STM32Cube F3 library V1.9.0](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubef3.html) to the `/opt/` directory.  You will need to install the following apt packages.

```bash
build-essential
cmake
gcc-arm-none-eabi
gdb-arm-none-eabi
```

To build the code.

```bash
mkdir firmware/nixie-clock/release
cd firmware/nixie-clock/release
cmake -DSTM32_CHIP=STM32F302C8 -DCMAKE_TOOLCHAIN_FILE=../../cmake/gcc_stm32.cmake -DSTM32Cube_DIR=/opt/STM32Cube_FW_F3_V1.9.0/ -DCMAKE_BUILD_TYPE=Release ..
make
```

To build for debugging

```bash
mkdir firmware/nixie-clock/debug
cd firmware/nixie-clock/debug
cmake -DSTM32_CHIP=STM32F302C8 -DCMAKE_TOOLCHAIN_FILE=../../cmake/gcc_stm32.cmake -DSTM32Cube_DIR=/opt/STM32Cube_FW_F3_V1.9.0/ -DCMAKE_BUILD_TYPE=Debug ..
make
```
