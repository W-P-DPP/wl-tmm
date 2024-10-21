#pragma once
#include "base/NonCopyable.h"
#include "EventLoop.h"
#include <vector>
#include "EventLoopThread.h"
#include <atomic>
namespace tmms
{
    namespace network
    {
        using EventLoopThreadPtr = std::shared_ptr<EventLoopThread>;
        class EventLoopThreadPool : public base::NonCopyable
        {
        private:
            std::vector<EventLoopThreadPtr> threads_;
            std::atomic_int32_t loop_index_{0};

        public:
            EventLoopThreadPool(int thread_num, int start = 0, int cpus = 2);
            ~EventLoopThreadPool();

            std::vector<EventLoop *> GetLoops() const;
            EventLoop *GetNextLoop();
            size_t Size();
            void Start();
        };

    }
}
