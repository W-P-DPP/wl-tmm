#include "TestContext.h"
#include <iostream>

using namespace tmms::network;
TestContext::TestContext(const TcpConnectionPtr &con)
    : connection_(con)
{
}

int TestContext::ParseMessage(MsgBuffer &buf)
{
    while (buf.readableBytes() > 0)
    {
        if (state_ == kTestContextHeader)
        {
            if (buf.readableBytes() >= 4)
            {
                message_length_ = buf.readInt32();
                std::cout << "message_length_:" << message_length_ << std::endl;
                state_ = kTestContextBody;
                continue;
            }
            else
            {
                return 1;
            }
        }
        else if (state_ == kTestContextBody)
        {
            if (buf.readableBytes() >= message_length_)
            {
                std::string tmp;
                tmp.assign(buf.peek(), message_length_);
                message_body_.append(tmp);
                buf.retrieve(message_length_);
                message_length_ = 0;
                if (cb_)
                {
                    cb_(connection_, message_body_);
                    message_body_.clear();
                }
                state_ = kTestContextHeader;
            }
        }
    }
    return 0;
}

void TestContext::SetTestMessageCallback(const TestMessageCallback &cb)
{
    cb_ = cb;
}

void TestContext::SetTestMessageCallback(TestMessageCallback &&cb)
{
    cb_ = std::move(cb);
}