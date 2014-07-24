cmake_minimum_required(VERSION 2.6)

PROJECT(indyva-cpp)


SET(INDYVACPP_MAJOR_VERSION 0)
SET(INDYVACPP_MINOR_VERSION 1)
SET(INDYVACPP_PATCH_VERSION 0)

SET (jsonrpc "${CMAKE_SOURCE_DIR}/vendor/libjson-rpc-cpp/build/local" CACHE PATH "Path to jsonrpc external project")
LINK_DIRECTORIES ( ${jsonrpc}/lib )
INCLUDE_DIRECTORIES ( ${jsonrpc}/include)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
 

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src)
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/vendor/libjson-rpc-cpp)
#INCLUDE(${CMAKE_SOURCE_DIR}/vendor/libjson-rpc-cpp/CMakeLists.txt)

# Con esto se compila entero libjson-rpc-cpp... pero fallan paths internos :(
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(examples)


