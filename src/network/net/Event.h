#pragma once
#include <string>
#include <memory>
#include <sys/epoll.h>
namespace tmms
{
    namespace network
    {

        class EventLoop;
        const int kEventRead = (EPOLLIN | EPOLLPRI | EPOLLET);
        const int kEventWrite = (EPOLLOUT | EPOLLET);
        class Event : public std::enable_shared_from_this<Event>
        {
            friend class EventLoop;

        protected:
            EventLoop *loop_{nullptr};
            int fd_{-1};
            int event_{0};

        public:
            Event(EventLoop *loop);
            Event(EventLoop *loop, int fd);
            virtual ~Event();

            virtual void OnRead() {};
            virtual void OnWrite() {};
            virtual void OnError(const std::string &err_msg) {};
            virtual void OnClose() {};
            bool EnableWriting(bool enable);
            bool EnableReading(bool enable);
            int Fd() const;
            void Close();
        };

    }
}