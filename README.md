# Infrastructure 

It's an infrastructure for universal C/C++ framework.

* [Intrastructure](http://gitlab.com/horance/infrastructure)

Supported Platform:
* [MAC OS X] supported
* [Linux] supported
* [Windows] not supported

Supported Compilers:
* [CLANG] 3.5 or later.
* [GCC] 4.9 or later.
* [MSVC] not supported.

Dependces for test cases:
* [Hamcrest](http://gitlab.com/horance/hamcrest).
* [Magellan](http://gitlab.com/horance/magellan).

## Installation

### Update GCC to 4.9

In order to support full C++11/14 features, to update GCC to 4.9 or later.

    sudo -E add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt-get update
    sudo apt-get install gcc-4.9 g++-4.9
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.9 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.9
    sudo update-alternatives --config gcc

### Install CMake:

    sudo apt-get install cmake
  
### Install Infrastructure

    mkdir build
    cd build
    cmake ..
    make ..
    sudo make install

### Install Hamcrest

    git clone https://gitlab.com/horance/hamcrest.git
    cd hamcrest
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

### Install Magellan:

    git clone https://gitlab.com/horance/magellan.git
    cd magellan
    mkdir build
    cd build
    cmake ..
    make
    sudo make install

## Copyright
Copyright (c) 2015-2020 Horance Liu. See LICENSE for details.
