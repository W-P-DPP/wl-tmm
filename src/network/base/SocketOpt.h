#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>
#include <netinet/tcp.h>
#include "InetAddress.h"
namespace tmms
{
    namespace network
    {
        using InetAddressPtr = std::shared_ptr<InetAddress>;
        class SocketOpt
        {
        private:
            int sock_{-1};
            bool is_v6_{false};

        public:
            SocketOpt(int sock, bool v6);
            ~SocketOpt() = default;

            static int CreateNonblockingTcpSocket(int family);
            static int CreateNonblockingUdpSocket(int family);

            //  server
            int BindAddress(const InetAddress &localaddr);
            int Listen();
            int Accept(InetAddress *peeraddr);

            // client
            int Connect(const InetAddress &addr);

            InetAddressPtr GEtLocalAddr();
            InetAddressPtr GEtpeerAddr();

            void SetTcpNonDelay(bool on);
            void SetReuseAddr(bool on);
            void SetReusePort(bool on);
            void SetKeepAlive(bool on);
            void SetNonBlocking(bool on);
        };

    }
}