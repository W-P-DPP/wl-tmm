#pragma once
#include "FileLog.h"
#include <memory>
#include "Singeton.h"
#include <unordered_map>
#include <mutex>

namespace tmms
{
    namespace base
    {

        class FileMgr : public NonCopyable
        {
        private:
            std::unordered_map<std::string, FileLogPtr> logs_;
            std::mutex lock_;
            int last_day_{-1};
            int last_hour_{-1};
            int last_minute_{-1};
            int last_year_{-1};
            int last_month_{-1};

        public:
            FileMgr() = default;
            ~FileMgr() = default;

            void OnCheck();
            FileLogPtr GetFileLog(const std::string &file_name);
            void RemoveFileLog(const FileLogPtr &log);
            void RotateDays(const FileLogPtr &log);
            void RotateHours(const FileLogPtr &log);
            void RotateMinute(const FileLogPtr &log);
        };

    }
}

#define sFileMgr tmms::base::Singeton<tmms::base::FileMgr>::Instance()