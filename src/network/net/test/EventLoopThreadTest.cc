#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include <iostream>

using namespace tmms::network;

EventLoopThread eventLoop_thread;
void TestEventLoopThread()
{
    eventLoop_thread.Run();
    EventLoop *loop = eventLoop_thread.Loop();

    if (loop)
    {
        std::cout << "loop:" << loop << std::endl;
    }
}

int main(int argv, char **argvc)
{

    TestEventLoopThread();
    return 0;
}
