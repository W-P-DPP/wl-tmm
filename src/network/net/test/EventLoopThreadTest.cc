#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/net/PipeEvent.h"
#include <iostream>
#include <thread>

using namespace tmms::network;

EventLoopThread eventLoop_thread;
void TestEventLoopThread()
{
    eventLoop_thread.Run();
    EventLoop *loop = eventLoop_thread.Loop();

    if (loop)
    {
        std::cout << "loop:" << loop << std::endl;
        PipeEventPrt pipe = std::make_shared<PipeEvent>(loop);
        loop->AddEvevt(pipe);
        int64_t test = 123456;
        pipe->Write((const char *)&test, sizeof(test));
    }
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main(int argv, char **argvc)
{

    TestEventLoopThread();
    return 0;
}
