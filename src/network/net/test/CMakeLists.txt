add_executable(EventLoopThreadTest EventLoopThreadTest.cc)
target_link_libraries(EventLoopThreadTest base network)


add_executable(InetAddressTest InetAddressTest.cc)
target_link_libraries(InetAddressTest base network)


add_executable(SocketOptTest SocketOptTest.cc)
target_link_libraries(SocketOptTest base network)

add_executable(AcceptorTest AcceptorTest.cc)
target_link_libraries(AcceptorTest base network)

add_executable(TcpConnectionTest TcpConnectionTest.cc)
target_link_libraries(TcpConnectionTest base network)

add_executable(TcpClientTest TcpClientTest.cc)
target_link_libraries(TcpClientTest base network)