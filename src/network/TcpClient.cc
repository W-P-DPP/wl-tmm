#include "TcpClient.h"
#include "network/base/Network.h"
#include "network/base/SocketOpt.h"

using namespace tmms::network;

TcpClient::TcpClient(EventLoop *loop, const InetAddress &server) : TcpConnection(loop, -1, InetAddress(), server), server_addr_(server) {};
TcpClient::~TcpClient()
{
    OnClose();
};

void TcpClient::Connect()
{
    loop_->RunInLoop([this]()
                     { ConnectInLoop(); });
};
void TcpClient::ConnectInLoop()
{
    loop_->AssertInLoopThread();
    fd_ = SocketOpt::CreateNonblockingTcpSocket(AF_INET);
    if (fd_ < 0)
    {
        OnClose();
        return;
    }
    status_ = kTcpConStatusConnecting;
    loop_->AddEvevt(std::dynamic_pointer_cast<TcpClient>(shared_from_this()));
    EnableWriting(true);
    SocketOpt opt(fd_);
    auto ret = opt.Connect(server_addr_);
    if (ret == 0)
    {
        UpdateConnectionStatus();
        return;
    }
    else if (ret == -1)
    {
        if (errno != EINPROGRESS)
        {
            NETWORK_ERROR << "连接到server失败，server：" << server_addr_.ToIpPort() << " errno:" << errno;
            OnClose();
            return;
        }
    }
};
void TcpClient::SetConnectCallback(ConnectionCallback &&cb)
{
    connect_cb_ = std::move(cb);
};
void TcpClient::SetConnectCallback(const ConnectionCallback &cb)
{
    connect_cb_ = cb;
};
void TcpClient::UpdateConnectionStatus()
{
    status_ = kTcpConStatusConnected;
    if (connect_cb_)
    {
        connect_cb_(std::dynamic_pointer_cast<TcpClient>(shared_from_this()), true);
    }
};
bool TcpClient::CheckError()
{
    int error = 0;
    socklen_t len = sizeof(error);
    ::getsockopt(fd_, SOL_SOCKET, SO_ERROR, &errno, &len);
    return error != 0;
};
void TcpClient::OnRead()
{
    if (status_ == kTcpConStatusConnecting)
    {
        if (CheckError())
        {
            NETWORK_ERROR << "连接到服务器：" << server_addr_.ToIpPort() << "失败，" << errno;
            OnClose();
            return;
        }
        UpdateConnectionStatus();
        return;
    }
    else if (status_ == kTcpConStatusConnected)
    {
        TcpConnection::OnRead();
    }
};

void TcpClient::OnWrite()
{
    if (status_ == kTcpConStatusConnecting)
    {
        if (CheckError())
        {
            NETWORK_ERROR << "连接到服务器：" << server_addr_.ToIpPort() << "失败，" << errno;
            OnClose();
            return;
        }
        UpdateConnectionStatus();
        return;
    }
    else if (status_ == kTcpConStatusConnected)
    {
        TcpConnection::OnWrite();
    }
};
void TcpClient::OnClose()
{
    if (status_ == kTcpConStatusConnecting || status_ == kTcpConStatusConnected)
    {
        loop_->DelEvevt(std::dynamic_pointer_cast<TcpClient>(shared_from_this()));
    }
    status_ = kTcpConStatusDisConnected;
    TcpConnection::OnClose();
};

void TcpClient::Send(std::list<BufferNodePtr> &list)
{
    if (status_ == kTcpConStatusConnected)
    {
        TcpConnection::Send(list);
    }
};
void TcpClient::Send(const char *buf, size_t size)
{
    if (status_ == kTcpConStatusConnected)
    {
        TcpConnection::Send(buf, size);
    }
};