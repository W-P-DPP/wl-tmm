add_executable(EventLoopThreadTest EventLoopThreadTest.cc)
target_link_libraries(EventLoopThreadTest base network)


add_executable(InetAddressTest InetAddressTest.cc)
target_link_libraries(InetAddressTest base network)


add_executable(SocketOptTest SocketOptTest.cc)
target_link_libraries(SocketOptTest base network)