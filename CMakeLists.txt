cmake_minimum_required(VERSION 2.6)

project(indyva_cpp)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake
  ${CMAKE_SOURCE_DIR}/CMake/common)
include(GitExternal)

include(ExternalProject)
ExternalProject_Add(libjson-rpc-cpp
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}/libjson-rpc-cpp"
  SOURCE_DIR "${CMAKE_SOURCE_DIR}/vendor/libjson-rpc-cpp"
  INSTALL_DIR "${CMAKE_BINARY_DIR}"
  BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/libjson-rpc-cpp"
  CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
             -DHTTP_CONNECTOR=OFF
             -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/install
             -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR})

set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "0")
set(VERSION_ABI 1)

set(PROJECT_INCLUDE_NAME indyva)

include(Common)

include(FindPackages)

link_directories(${CMAKE_BINARY_DIR}/install/lib)
include_directories(${CMAKE_BINARY_DIR}/include/)

add_subdirectory(indyva)
add_subdirectory(examples)

include(CPackConfig)
include(PackageConfig)


