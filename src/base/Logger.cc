
#include "Logger.h"
#include <iostream>

using namespace tmms::base;

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
    std::cout << str << std::endl;
};