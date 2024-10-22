#include "FileMgr.h"
#include "TTime.h"
#include "StringUtils.h"
#include <sstream>
#include <iostream>

using namespace tmms::base;

namespace
{
    static tmms::base::FileLogPtr file_log_nullptr;
}

void FileMgr::OnCheck()
{
    bool day_change{false};
    bool hour_change{false};
    bool minute_change{false};

    int year = 0,
        month = 0,
        day = -1,
        hour = -1,
        minute = 0,
        second = 0;
    TTime::Now(year, month, day, hour, minute, second);

    if (last_day_ == -1)
    {
        last_day_ = day;
        last_hour_ = hour;
        last_minute_ = minute;
        last_year_ = year;
        last_month_ = month;
    }
    if (last_day_ != day)
    {
        day_change = true;
    }
    if (last_hour_ != hour)
    {
        hour_change = true;
    }
    if (last_minute_ != minute)
    {
        minute_change = true;
    }
    std::cout << last_minute_ << " : " << minute << std::endl;
    if (!hour_change && !day_change && !minute_change)
        return;

    std::lock_guard<std::mutex> lk(lock_);

    for (auto &l : logs_)
    {
        if (minute_change && l.second->GetRotateTpe() == kRotateMinute)
        {
            RotateMinute(l.second);
        }
        if (hour_change && l.second->GetRotateTpe() == kRotateHour)
        {
            RotateHours(l.second);
        }
        if (day_change && l.second->GetRotateTpe() == kRotateDay)
        {
            RotateDays(l.second);
        }
    }
    last_day_ = day;
    last_hour_ = hour;
    last_year_ = year;
    last_month_ = month;
    last_minute_ = minute;
};
FileLogPtr FileMgr::GetFileLog(const std::string &file_name)
{
    std::lock_guard<std::mutex> lk(lock_);
    auto iter = logs_.find(file_name);
    if (iter != logs_.end())
    {
        return iter->second;
    }
    FileLogPtr log = std::make_shared<FileLog>();
    if (!log->Open(file_name))
    {
        return file_log_nullptr;
    }
    logs_.emplace(file_name, log);
    return log;
};
void FileMgr::RemoveFileLog(const FileLogPtr &log)
{
    std::lock_guard<std::mutex> lk(lock_);
    logs_.erase(log->FilePath());
};
void FileMgr::RotateDays(const FileLogPtr &file)
{
    if (file->FileSize() > 0)
    {
        char buf[128] = {
            0,
        };
        sprintf(buf, "_%04d_%02d_%02d", last_year_, last_month_, last_day_);
        std::string file_path = file->FilePath();
        std::string path = StringUtils::FilePath(file_path);
        std::string file_name = StringUtils::FileName(file_path);
        std::string file_exec = StringUtils::Extension(file_path);
        std::ostringstream oss;
        oss << path << file_name << file_exec;
        file->Rotate(oss.str());
    }
};
void FileMgr::RotateHours(const FileLogPtr &file)
{
    if (file->FileSize() > 0)
    {
        char buf[128] = {
            0,
        };
        sprintf(buf, "_%04d_%02d_%02dT%02d", last_year_, last_month_, last_day_, last_hour_);
        std::string file_path = file->FilePath();
        std::string path = StringUtils::FilePath(file_path);
        std::string file_name = StringUtils::FileName(file_path);
        std::string file_exec = StringUtils::Extension(file_path);

        std::ostringstream oss;
        oss << path << file_name << file_exec;
        file->Rotate(oss.str());
    }
};
void FileMgr::RotateMinute(const FileLogPtr &file)
{
    if (file->FileSize() > 0)
    {
        char buf[128] = {
            0,
        };
        sprintf(buf, "_%04d_%02d_%02dT%02d%02d", last_year_, last_month_, last_day_, last_hour_, last_minute_);
        std::string file_path = file->FilePath();
        std::string path = StringUtils::FilePath(file_path);
        std::string file_name = StringUtils::FileName(file_path);
        std::string file_exec = StringUtils::Extension(file_path);

        std::ostringstream oss;
        oss << path << file_name << file_exec;
        file->Rotate(oss.str());
    }
};
