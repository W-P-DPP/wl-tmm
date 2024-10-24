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
    // std::cout << "thread id:" << std::this_thread::get_id() << std::endl;
    // std::vector<EventLoop *> list = pool.GetLoops();
    // for (auto &e : list)
    // {
    //     e->RunInLoop([&e]()
    //                  { std::cout << "loop:" << e << "thread id:" << std::this_thread::get_id() << std::endl; });
    // }
    // EventLoop *loop = pool.GetNextLoop();
    // std::cout << "loop:" << loop << std::endl;
    EventLoop *loop = pool.GetNextLoop();
    std::cout << "loop:" << loop << std::endl;
    loop->RunAfter(1, []()
                   { std::cout << "RunAfter 1s now:" << tmms::base::TTime::NowMs() << std::endl; });
    loop->RunAfter(5, []()
                   { std::cout << "RunAfter 5s now:" << tmms::base::TTime::NowMs() << std::endl; });
    loop->RunEvery(1, []()
                   { std::cout << "RunEvery 1s now:" << tmms::base::TTime::NowMs() << std::endl; });
    loop->RunEvery(5, []()
                   { std::cout << "RunAfter 5s now:" << tmms::base::TTime::NowMs() << std::endl; });
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main(int argv, char **argvc)
{
    TestEventTLoophreadPool();
    // TestEventLoopThread();
    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
