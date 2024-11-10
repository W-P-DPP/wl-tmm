#pragma once
#include "network/base/InetAddress.h"
#include "network/net/TcpConnection.h"
#include "network/net/Acceptor.h"
#include "network/net/EventLoop.h"
#include <functional>
#include <memory>
#include <unordered_set>

namespace tmms
{
    namespace network
    {
        using NewConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
        using DestoryConnectionCallback = std::function<void(const TcpConnectionPtr &)>;
        class TcpServer
        {
        private:
            EventLoop *loop_{nullptr};
            std::shared_ptr<Acceptor> acceptor_;
            InetAddress addr_;
            NewConnectionCallback new_connection_cb_;
            std::unordered_set<TcpConnectionPtr> connections_;
            MessageCallback message_cb_;
            ActiveCallback active_cb_;
            WriteCompleteCallback write_complete_cb_;
            DestoryConnectionCallback destory_connection_cb;

        public:
            TcpServer(EventLoop *loop, const InetAddress &addr);
            virtual ~TcpServer();

            void SetNewConnectionCallback(NewConnectionCallback &&cb);
            void SetNewConnectionCallback(const NewConnectionCallback &cb);
            void SetDestoryConnectionCallback(DestoryConnectionCallback &&cb);
            void SetDestoryConnectionCallback(const DestoryConnectionCallback &cb);
            void SetActiveCallback(ActiveCallback &&cb);
            void SetActiveCallback(const ActiveCallback &cb);
            void SetWriteCompleteCallback(WriteCompleteCallback &&cb);
            void SetWriteCompleteCallback(const WriteCompleteCallback &cb);
            void SetMessageCallback(MessageCallback &&cb);
            void SetMessageCallback(const MessageCallback &cb);

            void OnAccept(int fd, const InetAddress &addr);

            void OnConnectionClose(const TcpConnectionPtr &con);

            virtual void Start();
            virtual void Stop();
        };

    }
}