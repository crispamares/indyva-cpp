
set(ECHO_SOURCES echo.cpp)
set(ECHO_LINK_LIBRARIES ${ZMQ_LIBRARIES} jsonrpc)
common_application(echo)

add_dependencies(echo libjson-rpc-cpp)
