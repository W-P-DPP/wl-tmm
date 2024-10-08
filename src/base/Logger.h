/**
 * @Desc: 日志
 * @Author: zwp
 * @Date: 2024-09-29 10:15:13
 */

#pragma once

#include "NonCopyable.h"
#include <string>

namespace tmms
{
    namespace base
    {
        enum LoggerLevel
        {
            kTrace,
            kDebug,
            kInfo,
            kWarn,
            kError,
            kMaxNumOfLoggerLevel
        };

        class Logger : NonCopyable
        {
        private:
            LoggerLevel level_{kDebug};

        public:
            Logger() = default;
            ~Logger() = default;

            void SetLoggerLevel(const LoggerLevel &level);
            LoggerLevel GetLoggerLevel() const;
            void Write(const std::string &str);
        };

    }
} // namespace tmms
