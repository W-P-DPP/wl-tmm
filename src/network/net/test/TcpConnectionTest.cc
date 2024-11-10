#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/net/TcpConnection.h"

#include <iostream>

using namespace tmms::network;

EventLoopThread eventLoop_thread;
std::thread th;

const char *http_response = "HTTP/1.0 200 OK\r\nServer: tmms\r\nContent-type: text/html\r\nContetn-Length: 0\r\n\r\n";
int main(int argc, const char **argv)
{
    eventLoop_thread.Run();
    EventLoop *loop = eventLoop_thread.Loop();

    if (loop)
    {
        std::vector<TcpConnectionPtr> list;
        InetAddress server("127.0.0.1:34444");
        std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(loop, server);
        acceptor->SetAcceptCallback([&loop, &server, &list](int fd, const InetAddress &addr)
                                    {
                                        std::cout << "host:" << addr.ToIpPort() << std::endl;
                                        TcpConnectionPtr connection = std::make_shared<TcpConnection>(loop, fd, server, addr);
                                        connection->SetRecMsgCallback([](const TcpConnectionPtr &conn, MsgBuffer &buf)
                                                                      {
                                            std::cout<<"recv msg:"<<buf.peek()<<std::endl;
                                            buf.retrieveAll();
                                            // conn->Send(http_response, strlen(http_response));
                                             });
                                            connection->SetWriteCompleteCallback([&loop](const TcpConnectionPtr &conn)
                                                                             {
                                            std::cout<<"write success host:"<<conn->PeerAddr().ToIpPort()<<std::endl;
                                            loop->DelEvent(conn);
                                                conn->ForceClose();
         });
         connection->EnableCheckIdleTimeout(3);
         list.push_back(connection);
        loop->AddEvent(connection); });
        acceptor->Start();
        while (1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}