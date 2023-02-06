FROM gcc:latest

ENV DEBIAN_FRONTEND noninteractive
LABEL maintainer="tomasvr"

WORKDIR /usr/local/bin
RUN apt-get update && apt-get install -y \
    cmake \
    gdb-multiarch \
    libnewlib-dev

RUN wget https://developer.arm.com/-/media/Files/downloads/gnu-a/9.2-2019.12/binrel/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi.tar.xz \
    && tar -xvf gcc-arm-9.2-2019.12-x86_64-arm-none-eabi.tar.xz
ENV PATH $PATH:/usr/local/bin/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/bin
# Create a symlink from the standard gcc-arm-none-eabi header files to the actual location fo the header files
RUN ln -s /usr/local/bin/gcc-arm-9.2-2019.12-x86_64-arm-none-eabi/lib/gcc/arm-none-eabi /usr/lib/gcc/arm-none-eabi