
#include "NonCopyable.h"
#include "Singeton.h"
#include <iostream>
#include "Logger.h"
#include "LogStream.h"

using namespace tmms::base;

class Test : public NonCopyable
{
private:
    /* data */
public:
    Test(/* args */) = default;
    ~Test() = default;

    void test()
    {
        std::cout << "TEST" << std::endl;
    }
};

void testLog()
{
    LOG_TRACE << "test LOG_TRACE";
    LOG_DEBUG << "test LOG_DEBUG";
    LOG_INFO << "test LOG_INFO";
    LOG_WARN << "test LOG_WARN";
    LOG_ERROR << "test LOG_ERROR";
}

int main(int arg, char *argv[])
{
    // auto test = Singeton<Test>::Instance();
    // test->test();
    g_logger = new Logger();
    g_logger->SetLoggerLevel(kTrace);
    testLog();
    return 0;
}
