# l0-infra

l0-infra is an infrastructure for universal C/C++ framework.

* [l0-infra](http://gitlab.com/horance/l0-infra)

Supported Platform:
* [MAC OS X] supported
* [Linux] supported
* [Windows] not supported

Supported Compilers:
* [CLANG] 3.5 or later.
* [GCC] 4.9 or later.
* [MSVC] not supported.

Dependences for test cases:
* [Magellan](http://gitlab.com/horance/magellan).

## Installation

### Install CMake:

    sudo apt-get install cmake
  
### Install l0-infra 

    mkdir build
    cd build
    cmake ..
    make ..
    sudo make install
    sudo ldconfig

### Install Magellan:

    git clone https://gitlab.com/horance/magellan.git
    cd magellan
    mkdir build
    cd build
    cmake ..
    make
    sudo make install
    sudo ldconfig

### Test l0-infra

    cd build
    cmake -DENABLE_TEST=on ..
    make
    test/l0-infra-test    

### l0-infra Developer

TO see [MDK](https://gitlab.com/horance/mdk) README for details.

## Copyright
Copyright (c) 2015-2020 Horance Liu. See LICENSE for details.
