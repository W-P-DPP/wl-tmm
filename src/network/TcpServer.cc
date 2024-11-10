#include "TcpServer.h"
#include "network/base/Network.h"

using namespace tmms::network;

TcpServer::TcpServer(EventLoop *loop, const InetAddress &addr) : loop_(loop), addr_(addr)
{
    acceptor_ = std::make_shared<Acceptor>(loop, addr);
};
TcpServer::~TcpServer() {};

void TcpServer::SetNewConnectionCallback(NewConnectionCallback &&cb)
{
    new_connection_cb_ = std::move(cb);
};
void TcpServer::SetNewConnectionCallback(const NewConnectionCallback &cb)
{
    new_connection_cb_ = cb;
};

void TcpServer::SetDestoryConnectionCallback(DestoryConnectionCallback &&cb)
{
    destory_connection_cb = std::move(cb);
};
void TcpServer::SetDestoryConnectionCallback(const DestoryConnectionCallback &cb)
{
    destory_connection_cb = cb;
};

void TcpServer::OnAccept(int fd, const InetAddress &addr)
{
    NETWORK_TRACE << "新连接,fd：" << fd << " host:" << addr.ToIpPort();
    TcpConnectionPtr con = std::make_shared<TcpConnection>(loop_, fd, addr_, addr);
    con->SetCloseCallback(std::bind(&TcpServer::OnConnectionClose, this, std::placeholders::_1));
    if (write_complete_cb_)
    {
        con->SetWriteCompleteCallback(write_complete_cb_);
    }
    if (active_cb_)
    {
        con->SetActiveCallback(active_cb_);
    }
    con->SetRecMsgCallback(message_cb_);
    connections_.insert(con);
    loop_->AddEvevt(con);
    con->EnableCheckIdleTimeout(30);
    if (new_connection_cb_)
    {
        new_connection_cb_(con);
    }
};

void TcpServer::OnConnectionClose(const TcpConnectionPtr &con)
{
    NETWORK_TRACE << "host:" << con->PeerAddr().ToIpPort() << " closed";
    loop_->AssertInLoopThread();
    connections_.erase(con);
    loop_->DelEvevt(con);
    if (destory_connection_cb)
    {
        destory_connection_cb(con);
    }
};

void TcpServer::SetActiveCallback(ActiveCallback &&cb)
{
    active_cb_ = std::move(cb);
};
void TcpServer::SetActiveCallback(const ActiveCallback &cb) { active_cb_ = cb; };
void TcpServer::SetWriteCompleteCallback(WriteCompleteCallback &&cb) { write_complete_cb_ = std::move(cb); };
void TcpServer::SetWriteCompleteCallback(const WriteCompleteCallback &cb) { write_complete_cb_ = cb; };
void TcpServer::SetMessageCallback(MessageCallback &&cb) { message_cb_ = std::move(cb); };
void TcpServer::SetMessageCallback(const MessageCallback &cb) { message_cb_ = cb; };

void TcpServer::Start()
{
    acceptor_->SetAcceptCallback(std::bind(&TcpServer::OnAccept, this, std::placeholders::_1, std::placeholders::_2));
    acceptor_->Start();
};
void TcpServer::Stop()
{
    acceptor_->Stop();
};