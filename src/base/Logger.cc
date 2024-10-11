
#include "Logger.h"
#include <iostream>

using namespace tmms::base;

Logger::Logger(const FileLogPtr &log) : log_{log} {};

void Logger::SetLoggerLevel(const LoggerLevel &level)
{
    level_ = level;
};
LoggerLevel Logger::GetLoggerLevel() const
{
    return level_;
};
void Logger::Write(const std::string &str)
{
    if (log_)
    {
        log_->WriteLog(str);
    }
    else
    {
        std::cout << str << std::endl;
    }
};