
# ---------- echo
set(ECHO_SOURCES echo.cpp)
set(ECHO_LINK_LIBRARIES ${ZMQ_LIBRARIES} jsonrpc indyva)
common_application(echo)

add_dependencies(echo libjson-rpc-cpp)

# ---------- pubsub
set(PUBSUB_SOURCES pubsub.cpp)
set(PUBSUB_LINK_LIBRARIES ${ZMQ_LIBRARIES} jsonrpc indyva)
common_application(pubsub)

add_dependencies(pubsub libjson-rpc-cpp)
