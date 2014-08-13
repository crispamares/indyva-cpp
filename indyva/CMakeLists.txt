include(files.cmake)

set(INDIVA_LINK_LIBRARIES ${ZMQ_LIBRARIES} jsonrpc)
common_library(indyva)
add_dependencies(indyva libjson-rpc-cpp)