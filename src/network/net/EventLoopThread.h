#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include "base/NonCopyable.h"
#include "EventLoop.h"
#include <future>
namespace tmms
{
    namespace network
    {
        class EventLoopThread : public base::NonCopyable
        {
        private:
            void StartEventLoop();
            EventLoop *loop_{nullptr};
            std::thread thread_;
            std::mutex lock_;
            std::condition_variable condition_;
            std::once_flag once_;
            std::promise<int> promise_loop_;
            bool running_{false};

        public:
            EventLoopThread(/* args */);
            ~EventLoopThread();
            void Run();
            EventLoop *Loop() const;
        };

    }

}
