#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"

#include <iostream>

using namespace tmms::network;

EventLoopThread eventLoop_thread;
std::thread th;

int main(int argc, const char **argv)
{
    eventLoop_thread.Run();
    EventLoop *loop = eventLoop_thread.Loop();

    if (loop)
    {
        InetAddress server("127.0.0.1:34444");
        std::shared_ptr<Acceptor> acceptor = std::make_shared<Acceptor>(loop, server);
        acceptor->SetAcceptCallback([](int fd, const InetAddress &addr)
                                    { std::cout << "host:" << addr.ToIpPort() << std::endl; });
        acceptor->Start();
        while (1)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    return 0;
}