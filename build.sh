#!/bin/bash

if [[ $1 = "target" ]]; then
    echo "Building target executable only"
    cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -S. -B_build/crosscompiled
    cmake --build _build/crosscompiled --target flash_targets
elif [[ $1 = "test" ]]; then
    echo "Building test executable only"
    cmake -DCMAKE_C_FLAGS="-fsanitize=undefined -ggdb3 -O0" -S. -B_build/native
    cmake --build _build/native --target utest
    ( cd _build/native && ./utest )
else
    echo "Building both test and target executable"
    cmake -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain-arm-none-eabi.cmake -S. -B_build/crosscompiled
    cmake --build _build/crosscompiled --target blackpill
    cmake -DCMAKE_C_FLAGS="-fsanitize=undefined -ggdb3 -O0" -S. -B_build/native
    cmake --build _build/native --target utest
    ( cd _build/native && ./utest )
fi