#include "network/net/EventLoop.h"
#include "network/net/EventLoopThread.h"
#include "network/net/EventLoopThreadPool.h"
#include "network/net/PipeEvent.h"
#include <iostream>
#include <thread>
#include "base/TTime.h"
using namespace tmms::network;

EventLoopThread eventLoop_thread;
std::thread th;
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
        th = std::thread([&pipe]()
                         {
            while (1)
            {
                int64_t now = tmms::base::TTime::NowMs();
                pipe->Write((const char *)&now, sizeof(now));
                std::this_thread::sleep_for(std::chrono::seconds(1));
            } });
    }
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void TestEventTLoophreadPool()
{
    EventLoopThreadPool pool(2, 0, 2);
    pool.Start();
    std::vector<EventLoop *> list = pool.GetLoops();
    for (auto &e : list)
    {
        std::cout << "loop:" << e << std::endl;
    }
    EventLoop *loop = pool.GetNextLoop();
    std::cout << "loop:" << loop << std::endl;
}

int main(int argv, char **argvc)
{
    TestEventTLoophreadPool();
    TestEventLoopThread();
    return 0;
}
