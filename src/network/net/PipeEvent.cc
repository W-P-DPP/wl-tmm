#include "PipeEvent.h"
#include <unistd.h>
#include <fcntl.h>
#include "network/base/Network.h"
#include <iostream>
#include <errno.h>

using namespace tmms::network;

PipeEvent::PipeEvent(EventLoop *loop) : Event(loop)
{
    int fd[2] = {
        0,
    };
    auto ret = ::pipe2(fd, O_NONBLOCK);
    if (ret < 0)
    {
        NETWORK_ERROR << "管道打开失败！";
        exit(-1);
    }
    fd_ = fd[0];
    write_fd_ = fd[1];
};
PipeEvent::~PipeEvent()
{
    if (write_fd_ > 0)
    {
        ::close(write_fd_);
        write_fd_ = -1;
    }
};
void PipeEvent::OnRead()
{
    int64_t tmp = 0;
    auto ret = ::read(fd_, &tmp, sizeof(tmp));
    if (ret < 0)
    {
        NETWORK_ERROR << "读取管道数据错误：" << fd_;
        return;
    }
    std::cout << "读取成功" << tmp << std::endl;
};
void PipeEvent::OnError(const std::string &err_msg)
{
    std::cout << "error: " << err_msg << std::endl;
};
void PipeEvent::OnClose()
{
    if (write_fd_ > 0)
    {
        ::close(write_fd_);
        write_fd_ = -1;
    }
};

void PipeEvent::Write(const char *data, size_t len)
{
    auto ret = ::write(write_fd_, data, len);
};