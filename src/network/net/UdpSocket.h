#pragma once
#include "network/base/InetAddress.h"
#include "network/base/MsgBuffer.h"
#include "EventLoop.h"
#include <list>
#include <functional>
#include <memory>
#include "Connection.h"

namespace tmms
{
    namespace network
    {
        class UdpSocket;
        using UdpSocketPtr = std::shared_ptr<UdpSocket>;
        using UdpMessageCallback = std::function<void(const InetAddress &addr, MsgBuffer &buf)>;
        using UdpSocketWriteCompleteCallback = std::function<void(const UdpSocketPtr &)>;
        using UdpSocketCloseConnectionCallback = std::function<void(const UdpSocketPtr &)>;
        using UdpSocketTimeoutCallback = std::function<void(const UdpSocketPtr &)>;
        struct UdpTimeoutEntry;
        struct UdpBufferNode : public BufferNode
        {
            UdpBufferNode(void *buf, size_t s, struct sockaddr *saddr, socklen_t len)
                : BufferNode(buf, s), addr(saddr), sock_len(len)
            {
            }
            struct sockaddr *addr{nullptr};
            socklen_t sock_len{0};
        };
        using UdpBufferNodePtr = std::shared_ptr<UdpBufferNode>;
        class UdpSocket : public Connection
        {
        public:
            UdpSocket(EventLoop *loop, int fd, const InetAddress &localAddr, const InetAddress &peerAddr);
            virtual ~UdpSocket();
            void OnTimeout();

            void OnError(const std::string &msg) override;
            void OnRead() override;
            void OnWrite() override;
            void OnClose() override;

            void SetCloseCallback(const UdpSocketCloseConnectionCallback &cb);
            void SetCloseCallback(UdpSocketCloseConnectionCallback &&cb);
            void SetRecvMsgCallback(const UdpMessageCallback &cb);
            void SetRecvMsgCallback(UdpMessageCallback &&cb);
            void SetWriteCompleteCallback(const UdpSocketWriteCompleteCallback &cb);
            void SetWriteCompleteCallback(UdpSocketWriteCompleteCallback &&cb);
            void SetTimeoutCallback(int timeout, const UdpSocketTimeoutCallback &cb);
            void SetTimeoutCallback(int timeout, UdpSocketTimeoutCallback &&cb);
            void EnableCheckIdleTimeout(int32_t max_time);
            void Send(std::list<UdpBufferNodePtr> &list);
            void Send(const char *buf, size_t size, struct sockaddr *addr, socklen_t len);
            void SendInLoop(std::list<UdpBufferNodePtr> &list);
            void SendInLoop(const char *buf, size_t size, struct sockaddr *saddr, socklen_t len);
            void ForceClose() override;

        private:
            void EntendLife();
            std::list<UdpBufferNodePtr> buffer_list_;
            bool closed_{false};
            int32_t max_idle_time_{30};
            std::weak_ptr<UdpTimeoutEntry> timeout_entry_;
            int32_t message_buffer_size_{65535};
            MsgBuffer message_buffer_;
            UdpMessageCallback message_cb_;
            UdpSocketWriteCompleteCallback write_complete_cb_;
            UdpSocketCloseConnectionCallback close_cb_;
        };

        struct UdpTimeoutEntry
        {
        public:
            UdpTimeoutEntry(const UdpSocketPtr &c)
                : conn(c)
            {
            }
            ~UdpTimeoutEntry()
            {
                auto c = conn.lock();
                if (c)
                {
                    c->OnTimeout();
                }
            }
            std::weak_ptr<UdpSocket> conn;
        };
    }
}