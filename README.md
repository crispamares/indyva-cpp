# indyva-cpp

## Installation

You need `cmake` installed and zmq:

    sudo apt-get install cmake
    sudo apt-get install libzmq-dev
   
In the root of this project 

    git submodule init
    git submodule update

Then build the subproject

    cd vendor/libjson-rpc-cpp/build  
    cmake ../ -DHTTP_CONNECTOR=OFF -DCMAKE_INSTALL_PREFIX=local
    make 
    make install    # installed in vendor/libjson-rpc-cpp/build/local

Finally, you can build the project. In the root of the project

    mkdir build
    cd build
    cmake ..
    make 

## The echo example

This example shows how to create a client that speaks to an Indyva server.
