#include "PipeEvent.h"
#include <unistd.h>
#include <fcntl.h>

using namespace tmms::network;

PipeEvent::PipeEvent(EventLoop *loop) : Event(loop)
{
    int fd[2] = {
        0,
    };
    auto ret = ::pipe2(fd, O_NONBLOCK);
};
PipeEvent::~PipeEvent() {};
void PipeEvent::OnRead() {};
void PipeEvent::OnError(const std::string &err_msg) {};
void PipeEvent::OnClose() {};

void PipeEvent::Write(const char *data, size_t len) {};