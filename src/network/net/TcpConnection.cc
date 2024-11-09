#include "TcpConnection.h"
#include "network/base/Network.h"
#include <unistd.h>
#include <iostream>
using namespace tmms::network;

TcpConnection::TcpConnection(EventLoop *loop, int socketfd, const InetAddress &localAddr, const InetAddress &peerAddr)
    : Connection(loop, socketfd, localAddr, peerAddr) {};
TcpConnection::~TcpConnection() { OnClose(); };

void TcpConnection::OnClose()
{

    loop_->AssertInLoopThread();
    if (!closed_)
    {
        if (close_cb_)
        {
            close_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
        closed_ = true;
        Event::Close();
    }
};
void TcpConnection::ForceClose()
{
    loop_->RunInLoop([this]()
                     { OnClose(); });
};

void TcpConnection::SetCloseCallback(const CloseConnectionCallback &cb)
{
    close_cb_ = cb;
};
void TcpConnection::SetCloseCallback(CloseConnectionCallback &&cb)
{
    close_cb_ = std::move(cb);
};

void TcpConnection::SetRecMsgCallback(const MessageCallback &cb)
{
    message_cb_ = cb;
};
void TcpConnection::SetRecMsgCallback(MessageCallback &&cb)
{
    message_cb_ = std::move(cb);
};

void TcpConnection::OnRead()
{
    if (closed_)
    {
        NETWORK_ERROR << "连接已关闭,host:" << peer_addr_.ToIpPort();
        return;
    }
    while (true)
    {
        int err = 0;
        auto ret = message_buffer_.readFd(fd_, &err);
        if (ret > 0)
        {
            if (message_cb_)
            {
                message_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()), message_buffer_);
            }
        }
        else if (ret == 0)
        {
            OnClose();
            break;
        }
        else
        {
            if (err != EINTR && err != EAGAIN && err != EWOULDBLOCK)
            {
                NETWORK_ERROR << "读取错误:" << err;
                OnClose();
            }

            break;
        }
    }
};

void TcpConnection::OnError(const std::string &msg)
{
    NETWORK_ERROR << "host:" << peer_addr_.ToIpPort() << "出错了：" << msg;
    OnClose();
};

void TcpConnection::OnWrite()
{
    if (closed_)
    {
        NETWORK_TRACE << "连接以关闭，host:" << peer_addr_.ToIpPort();
        return;
    }

    if (!io_vec_li_.empty())
    {
        while (true)
        {
            auto ret = ::writev(fd_, &io_vec_li_[0], io_vec_li_.size());
            if (ret >= 0)
            {
                while (ret > 0)
                {
                    if (io_vec_li_.front().iov_len > ret)
                    {
                        io_vec_li_.front().iov_base = (char *)io_vec_li_.front().iov_base + ret;
                        io_vec_li_.front().iov_len -= ret;
                        break;
                    }
                    else
                    {
                        ret -= io_vec_li_.front().iov_len;
                        io_vec_li_.erase(io_vec_li_.begin());
                    }
                }
                if (io_vec_li_.empty())
                {
                    EnableWriting(false);
                    if (write_complete_cb_)
                    {
                        write_complete_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
                    }
                    return;
                }
            }
            else
            {

                if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
                {
                    NETWORK_ERROR << "写出错，host:" << peer_addr_.ToIpPort() << ";errno:" << errno;
                    OnClose();
                    return;
                }
                break;
            }
        }
    }
    else
    {
        EnableWriting(false);
        if (write_complete_cb_)
        {
            write_complete_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
    }
};
void TcpConnection ::SetWriteCompleteCallback(const WriteCompleteCallback &cb)
{
    write_complete_cb_ = cb;
};
void TcpConnection::SetWriteCompleteCallback(WriteCompleteCallback &&cb)
{
    write_complete_cb_ = std::move(cb);
};
void TcpConnection::Send(std::list<BufferNodePtr> &list)
{
    loop_->RunInLoop([this, &list]()
                     { SendInLoop(list); });
};
void TcpConnection::Send(const char *buf, size_t size)
{
    loop_->RunInLoop([this, buf, size]()
                     { SendInLoop(buf, size); });
};
void TcpConnection::SendInLoop(std::list<BufferNodePtr> &list)
{
    if (closed_)
    {
        NETWORK_ERROR << "连接已关闭,host:" << peer_addr_.ToIpPort();
        return;
    }
    for (auto &l : list)
    {
        struct iovec vec;
        vec.iov_base = (void *)l->addr;
        vec.iov_len = l->size;
        io_vec_li_.push_back(vec);
    }
    if (!io_vec_li_.empty())
    {
        EnableWriting(true);
    }
};
void TcpConnection::SendInLoop(const char *buf, size_t size)
{
    if (closed_)
    {
        NETWORK_ERROR << "连接已关闭,host:" << peer_addr_.ToIpPort();
        return;
    }
    size_t send_len = 0;
    if (io_vec_li_.empty())
    {
        send_len = ::write(fd_, buf, size);
        if (send_len < 0)
        {
            if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK)
            {
                NETWORK_ERROR << "写出错，host:" << peer_addr_.ToIpPort() << ";errno:" << errno;
                OnClose();
                return;
            }
            send_len = 0;
        }
        size -= send_len;
    }
    if (size == 0)
    {
        if (write_complete_cb_)
        {
            write_complete_cb_(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
        }
        return;
    }
    if (size > 0)
    {
        struct iovec vec;
        vec.iov_base = (void *)(buf + send_len);
        vec.iov_len = size;
        io_vec_li_.push_back(vec);
        EnableWriting(true);
    }
};

void TcpConnection::SetTimeoutCallback(int timeout, const TimeoutCallback &cb)
{
    auto cp = std::dynamic_pointer_cast<TcpConnection>(shared_from_this());
    loop_->RunAfter(timeout, [&cp, &cb]()
                    { cb(cp); });
};
void TcpConnection::SetTimeoutCallback(int timeout, TimeoutCallback &&cb)
{
    auto cp = std::dynamic_pointer_cast<TcpConnection>(shared_from_this());
    loop_->RunAfter(timeout, [&cp, &cb]()
                    { cb(cp); });
};
void TcpConnection::OnTimeout()
{
    NETWORK_ERROR << "超时关闭，host:" << peer_addr_.ToIpPort();
    std::cout << "超时关闭，host:" << peer_addr_.ToIpPort()
              << std::endl;
    OnClose();
};

void TcpConnection::EnableCheckIdleTimeout(int32_t max_time)
{
    auto tp = std::make_shared<TimeoutEntry>(std::dynamic_pointer_cast<TcpConnection>(shared_from_this()));
    max_idle_time_ = max_time;
    timeout_entry_ = tp;
    loop_->InsertEntry(max_time, tp);
};
void TcpConnection::ExtendLife()
{
    auto tp = timeout_entry_.lock();
    if (tp)
    {
        loop_->InsertEntry(max_idle_time_, tp);
    }
};