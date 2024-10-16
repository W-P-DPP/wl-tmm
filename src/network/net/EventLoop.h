#pragma once
#include <vector>
#include <sys/epoll.h>
#include "Event.h"
#include <memory>
#include <unordered_map>
namespace tmms
{
    namespace network
    {
        using EventPtr = std::shared_ptr<Event>;
        class EventLoop
        {
        private:
            bool looping_{false};
            int epoll_fd_{-1};
            std::vector<struct epoll_event> epoll_events_;
            std::unordered_map<int, EventPtr> events_;

        public:
            EventLoop();
            ~EventLoop();

            void Loop();
            void Quit();

            void AddEvevt(const EventPtr &event);
            void DelEvevt(const EventPtr &event);
            bool EnableEventWriting(const EventPtr &event, bool enable);
            bool EnableEventReading(const EventPtr &event, bool enable);
        };

    }
}
