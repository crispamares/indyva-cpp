# indyva-cpp

## Installation

You need `cmake` installed and `zmq`:

    sudo apt-get install cmake
    sudo apt-get install libzmq3-dev

Then, you can build the project. In the root of the project

    mkdir build
    cd build
    cmake ..
    make

The library libjson-rpc-cpp will be automatically downloaded during the
configuration process and compiled by the build script.

## The echo example

This example shows how to create a client that speaks to an Indyva server.
