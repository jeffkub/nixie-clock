# nixie-clock

An open source nixie clock with GPS time synchronization.

## Enclosure

The enclosure design has been moved to Fusion 360.  The files in the hardware folder are out of date and will be removed at some point.

1. [Enclosure](http://a360.co/2x962o5)
2. [PCB](http://a360.co/2x9v2f3)
3. [Assembly](http://a360.co/2y7SgBr)

![alt text](/pictures/3d_render.png?raw=true)

## Build Instructions

The code was built on Ubuntu 16.04.  The [STM32Cube F3 library V1.9.0](http://www.st.com/content/st_com/en/products/embedded-software/mcus-embedded-software/stm32-embedded-software/stm32cube-embedded-software/stm32cubef3.html) will need to be installed to the `/opt/` directory.  The following apt packages are required.

```
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
