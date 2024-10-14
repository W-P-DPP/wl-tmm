
#include "NonCopyable.h"
#include "Singeton.h"
#include <iostream>
#include "Logger.h"
#include "LogStream.h"
#include "FileMgr.h"
#include <thread>
#include "TTime.h"
#include "TaksMgr.h"
#include <iostream>

using namespace tmms::base;
std::thread t;

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
    };
};

void testLog()
{
    t = std::thread([]()
                    {
                        while (true)
                        {
                                            
         LOG_TRACE << "test LOG_TRACE"<< TTime::NowMs();
    LOG_DEBUG << "test LOG_DEBUG"<< TTime::NowMs();
    LOG_INFO << "test LOG_INFO"<< TTime::NowMs();
    LOG_WARN << "test LOG_WARN"<< TTime::NowMs();
    LOG_ERROR << "test LOG_ERROR"<< TTime::NowMs();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                        } });
}

int main(int arg, char *argv[])
{
    // auto test = Singeton<Test>::Instance();
    // test->test();
    FileLogPtr log = sFileMgr->GetFileLog("test.log");
    log->SetRotate(kRotateMinute);
    g_logger = new Logger(log);
    g_logger->SetLoggerLevel(kTrace);
    TaskPtr task4 = std::make_shared<Task>([](const TaskPtr &prt)
                                           { sFileMgr->OnCheck(); },
                                           1000);
    sTaskMgr->Add(task4);
    testLog();
    while (1)
    {
        sTaskMgr->OnWork();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 0;
}
