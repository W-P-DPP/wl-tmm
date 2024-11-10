#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include <iostream>
#include "network/UdpClient.h"

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
        InetAddress server("127.0.0.1:34444");
        std::shared_ptr<UdpClient> client = std::make_shared<UdpClient>(loop, server);
        client->SetRecvMsgCallback([](const InetAddress &addr, MsgBuffer &buf)
                                   {
            std::cout << "host:" << addr.ToIpPort() <<"msg:" << buf.peek() <<  std::endl;
            buf.retrieveAll(); });
        client->SetCloseCallback([](const UdpSocketPtr &con)
                                 { std::cout << "host:" << con->PeerAddr().ToIpPort() << "close." << std::endl; });
        client->SetWriteCompleteCallback([](const UdpSocketPtr &con)
                                         {
            if (con)
            {
                std::cout << "host:" << con->PeerAddr().ToIpPort() << "write complete."<< std::endl;
            } });
        client->SetConnectedCallback([&client](const UdpSocketPtr &con, bool connected)
                                     {
            if (connected)
            {
                std::cout << "connect cb" << std::endl;
                client->Send("11111",strlen("11111"));
            } });
        client->Connect();
        while (1)
        {

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}