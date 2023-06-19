# TDD ARM development (Gtest, libopencm3)

This repository is a sample project for Test Driven Development on ARM microcontrollers using libopencm3 and gtest.
It also includes a dockerfile to quickly set up the development environment and toolchain.
The project is currently configured for the STM32 blackpill project. However, the same setup can be used for a wide range of ARM processors supported by libopencm3.

# Setup

The setup can be done either automatic using Docker or manually by installing the required dependencies yourself.
First clone the repository to your desired location.

Initalize the submodules either during cloning:
```
git clone --recurse-submodules git://github.com/tomasvr/arm-gtest.git
```

Or after cloning:
```
git submodule update --init
```

## Docker Setup (recommended)

from the root directory of this repository run:
```
sudo docker build -t tomasvr/arm-gtest .
sudo docker run --rm -it --device=/dev/ttyACM0 -v "$(pwd):/home/app" tomasvr/arm-gtest
```
The --device flags allows us to access the device at `/dev/ttyACM0` where the blackpill is usually located. <br>
**Note**: if your serial port connection is located at a different device file then you have to change `/dev/ttyACM0` to your serial port location in the above docker command and also in the `cmake/target.cmake` script for flashing.

## Manual Setup

Dependencies:
* gcc-arm-none-eabi 9.2.1
* gcc, g++
* cmake
* libnewlib-dev
* gdb-multiarch (for flashing)

Install the require arm toolchain locally:
```
    cd /usr/local/binwget && https://developer.arm.com/-/media/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi.tar.xz \
    && tar -xvf gcc-arm-9.2-2019.12-x86_64-arm-none-eabi.tar.xz
export PATH=$PATH:/usr/local/bin/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/bin
```
Create a symlink from the standard gcc-arm-none-eabi header files to the actual location of the header files
```
ln -s /usr/local/bin/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/lib/gcc/arm-none-eabi /usr/lib/gcc/arm-none-eabi
```
If you are done with this project and already had a version of gcc-arm-none-eabi that you want to use again, delete this symlink.

Install the remaining dependencies:
```
sudo apt install -y cmake libnewlib-dev gdb-multiarch
```

## Building

Now simply run the build script to build the target binary and test executable:
```
./build.sh
```
The target binary, elf file and other artifacts produced will be located in the `_build/crosscompiled` directory. <br>
The test executable will be located in the `_build/native` directory. The test executable will automatically be executed after being built.

To build only the target binary and flash it onto the MCU (assuming the relevant device file is: `/dev/ttyACM0`):
```
./build.sh target
```
To build only the test executable:
```
./build.sh test
```

## Using different MCUs

There a few files that need some changes to configure the project for a new MCU.

In `cmake/libopencm3_stm32f1` change `MCU_ARCH`, `MCU_OPENCM3_NAME` and `MCU_NAME` to your MCU according to libopencm3 naming conventions.<br>Also change `MCU_LINKER_SCRIPT` to point to the location of the linker script for your MCU. <br>

In `src/target/main.c` Change the included libopencm3 headers to match your MCU.

And that's all!

## Project structure

* `src/`: contains the source files of the project to be tested
* `lib/`: contains the external libraries (gtest and libopencm3) used for this project
* `test/`: contains the source code for the test cases and mocks used for unit testing
* `_build/crosscompiled`: contains the target artifacts (flashable binary, elf file)
* `_build/native`: contains the test artifacts (test executable)
* `cmake`: contains the different cmake files which need to be modified per MCU
