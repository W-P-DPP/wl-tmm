#include "network/base/SocketOpt.h"
#include "network/base/InetAddress.h"
#include <string>
#include <iostream>
using namespace tmms::network;

void TestClient()
{
    int sock = SocketOpt::CreateNonblockingTcpSocket(AF_INET);
    if (sock < 0)
    {
        std::cerr << "socket failed.sock:" << sock << "errno:" << errno << std::endl;
        return;
    }

    InetAddress server("192.168.22.185:34444");
    SocketOpt opt(sock, false);
    opt.SetNonBlocking(true);
    auto ret = opt.Connect(server);
    std::cout << "connect ret:" << std::endl
              << ret << " errno:" << errno << " local:" << opt.GEtLocalAddr() << std::endl
              << " peer:" << opt.GEtpeerAddr() << std::endl
              << std::endl;
}

int main(int argc, char **argv)
{
    int sock = SocketOpt::CreateNonblockingTcpSocket(AF_INET);
    if (sock < 0)
    {
        std::cerr << "socket failed.sock:" << sock << "errno:" << errno << std::endl;
        return -1;
    }

    InetAddress server("0.0.0.0:34444");
    SocketOpt opt(sock, false);
    opt.SetNonBlocking(false);
    opt.BindAddress(server);
    opt.Listen();
    InetAddress addr;
    auto ns = opt.Accept(&addr);
    std::cout << "Accept ret:" << std::endl
              << ns << " errno:" << errno << " addr:" << addr.ToIpPort() << std::endl
              << std::endl;
    return 0;
}