#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include <iostream>
#include "network/UdpServer.h"

using namespace tmms::network;
EventLoopThread eventloop_thread;
std::thread th;

const char *http_response = "HTTP/1.0 200 OK\r\nServer: tmms\r\nContent-Type: text/html\r\nContent-Length: 0\r\n\r\n";
int main(int argc, const char *argv[])
{
    eventloop_thread.Run();
    EventLoop *loop = eventloop_thread.Loop();
    if (loop)
    {
        InetAddress listen("127.0.0.1:34444");
        std::shared_ptr<UdpServer> server = std::make_shared<UdpServer>(loop, listen);
        server->SetRecvMsgCallback([server](const InetAddress &addr, MsgBuffer &buf)
                                   {
            std::cout << "host:" << addr.ToIpPort() <<"msg:" << buf.peek() <<  std::endl;
            struct sockaddr_in6 sock_addr;
            addr.GetSockAddr((struct sockaddr*)&sock_addr);
            server->Send(buf.peek(), buf.readableBytes(), (struct sockaddr*)&sock_addr, sizeof(struct sockaddr_in6));
            buf.retrieveAll(); });
        server->SetCloseCallback([](const UdpSocketPtr &con)
                                 { std::cout << "host:" << con->PeerAddr().ToIpPort() << "close." << std::endl; });
        server->SetWriteCompleteCallback([](const UdpSocketPtr &con)
                                         {
            if (con)
            {
                std::cout << "host:" << con->PeerAddr().ToIpPort() << "write complete."<< std::endl;
            } });

        server->Start();
        while (1)
        {

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}