#pragma once
#include <vector>
#include <sys/epoll.h>
#include "Event.h"
#include <memory>
#include <unordered_map>
#include <functional>
#include <queue>
#include <mutex>
#include "PipeEvent.h"

namespace tmms
{
    namespace network
    {
        using EventPtr = std::shared_ptr<Event>;
        using Func = std::function<void()>;
        class EventLoop
        {
        private:
            bool looping_{false};
            int epoll_fd_{-1};
            std::vector<struct epoll_event> epoll_events_;
            std::unordered_map<int, EventPtr> events_;

            std::queue<Func> functions_;
            std::mutex lock_;

            PipeEventPrt pipe_event_;
            // void QueueInLoop(const Func &f);
            // void QueueInLoop(Func &&f);
            void RunFunctions();

        public:
            EventLoop();
            ~EventLoop();

            void Loop();
            void Quit();
            void WakeUp();
            void AddEvevt(const EventPtr &event);
            void DelEvevt(const EventPtr &event);
            bool EnableEventWriting(const EventPtr &event, bool enable);
            bool EnableEventReading(const EventPtr &event, bool enable);

            void AssertInLoopThread();
            bool IsInLoopThread() const;
            void RunInLoop(const Func &f);
            void RunInLoop(Func &&f);
        };

    }
}
