#pragma once
#include "network/net/TcpConnection.h"
#include "network/base/InetAddress.h"
#include <functional>
#include "network/net/EventLoop.h"
#include <list>
#include "network/base/MsgBuffer.h"

namespace tmms
{
    namespace network
    {
        enum
        {
            kTcpConStatusInit = 0,
            kTcpConStatusConnecting = 1,
            kTcpConStatusConnected = 2,
            kTcpConStatusDisConnected = 3,
        };
        using ConnectionCallback = std::function<void(const TcpConnectionPtr &conn, bool)>;
        class TcpClient : public TcpConnection
        {
        public:
            TcpClient(EventLoop *loop, const InetAddress &server);
            virtual ~TcpClient();

            void Connect();
            void SetConnectCallback(ConnectionCallback &&cb);
            void SetConnectCallback(const ConnectionCallback &cb);

            void OnClose() override;
            void OnRead() override;
            void OnWrite() override;

            void Send(std::list<BufferNodePtr> &list);
            void Send(const char *buf, size_t size);

        private:
            bool CheckError();
            void ConnectInLoop();
            void UpdateConnectionStatus();
            InetAddress server_addr_;
            ConnectionCallback connect_cb_;
            int32_t status_;
        };
    }
}