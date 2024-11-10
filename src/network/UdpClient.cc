#include "UdpClient.h"
#include <network/base/Network.h>
#include <iostream>
#include "network/base/SocketOpt.h"

using namespace tmms::network;
UdpClient::UdpClient(EventLoop *loop, const InetAddress &server)
    : UdpSocket(loop, -1, InetAddress(), server), server_addr_(server)
{
}

UdpClient::~UdpClient()
{
}

void UdpClient::Connect()
{
    loop_->RunInLoop([this]()
                     { ConnectInLoop(); });
}

void UdpClient::SetConnectedCallback(const ConnectedCallback &cb)
{
    connected_cb_ = cb;
}

void UdpClient::SetConnectedCallback(ConnectedCallback &&cb)
{
    connected_cb_ = std::move(cb);
}

void UdpClient::ConnectInLoop()
{
    loop_->AssertInLoopThread();
    fd_ = SocketOpt::CreateNonblockingUdpSocket(AF_INET);
    if (fd_ < 0)
    {
        OnClose();
        return;
    }
    connected_ = true;
    loop_->AddEvent(std::dynamic_pointer_cast<UdpClient>(shared_from_this()));
    EnableWriting(true);
    SocketOpt opt(fd_);
    std::cout << "Connect" << std::endl;
    opt.Connect(server_addr_);
    std::cout << "Connect2" << std::endl;
    server_addr_.GetSockAddr((struct sockaddr *)&sock_addr_);
    if (connected_cb_)
    {
        connected_cb_(std::dynamic_pointer_cast<UdpClient>(shared_from_this()), true);
    }
}

void UdpClient::Send(std::list<UdpBufferNodePtr> &list)
{
    UdpSocket::Send(list);
}

void UdpClient::Send(const char *buf, size_t size)
{
    UdpSocket::Send(buf, size, (struct sockaddr *)&sock_addr_, sock_len_);
}

void UdpClient::OnClose()
{
    if (connected_)
    {
        loop_->DelEvent(std::dynamic_pointer_cast<UdpClient>(shared_from_this()));
        connected_ = false;
        UdpSocket::OnClose();
    }
}