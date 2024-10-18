#pragma once

#include "Event.h"

namespace tmms
{
    namespace network
    {
        class PipeEvent : public Event
        {
        private:
            int write_fd_{-1};

        public:
            PipeEvent(EventLoop *loop);
            ~PipeEvent();

            void OnRead() override;
            void OnError(const std::string &err_msg) override;
            void OnClose() override;

            void Write(const char *data, size_t len);
        };

    }
}