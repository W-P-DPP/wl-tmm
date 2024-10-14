#include "Config.h"
#include "LogStream.h"
#include <fstream>
using namespace tmms ::base;

bool Config::LoadConfig(const std::string &file)
{
    LOG_DEBUG << "配置文件路径：" << file;
    Json::Value root;
    Json::CharReaderBuilder reader;
    std::ifstream in(file);
    std::string err;
    bool ok = Json::parseFromStream(reader, in, &root, &err);
    if (!ok)
    {
        LOG_ERROR << "配置文件解析错误：" << file;
        return false;
    }
    Json::Value nameObj = root["name"];
    if (!nameObj.isNull())
    {
        name_ = nameObj.asString();
    }
    Json::Value cupsObj = root["cup_start"];
    if (!cupsObj.isNull())
    {
        cpu_start_ = cupsObj.asInt();
    }
    Json::Value threadsObj = root["threads"];
    if (!threadsObj.isNull())
    {
        thread_num_ = threadsObj.asInt();
    }
    Json::Value logObj = root["log"];
    if (!logObj.isNull())
    {
        ParseLogInfo(logObj);
    }
    return true;
};
bool Config::ParseLogInfo(const Json::Value &root)
{
    log_info_ = std::make_shared<LogInfo>();
    Json::Value levelObj = root["level"];
    if (!levelObj.isNull())
    {
        std::string level = levelObj.asString();
        if (level == "TRACE")
        {
            log_info_->level = kTrace;
        }
        else if (level == "DEBUG")
        {
            log_info_->level = kDebug;
        }
        else if (level == "ERROR")
        {
            log_info_->level = kError;
        }
        else if (level == "INFO")
        {
            log_info_->level = kInfo;
        }
        else if (level == "WARN")
        {
            log_info_->level = kWarn;
        }
    };

    Json::Value pathObj = root["path"];
    if (!pathObj.isNull())
    {
        log_info_->path = pathObj.asString();
    }
    Json::Value nameObj = root["name"];
    if (!nameObj.isNull())
    {
        log_info_->name = nameObj.asString();
    }

    Json::Value rtObj = root["rotate"];
    if (!rtObj.isNull())
    {
        std::string rt = rtObj.asString();
        if (rt == "Day")
        {
            log_info_->rotate_type = kRotateDay;
        }
        else if (rt == "HOUR")
        {
            log_info_->rotate_type = kRotateHour;
        }
    }

    if (levelObj.isNull() || pathObj.isNull() || nameObj.isNull())
    {
        return false;
    }

    return true;
};
LogInfoPtr &Config::GetLogInfo()
{
    return log_info_;
};

bool ConfigMgr::LoadConfig(const std::string &file)
{
    LOG_DEBUG << "加载配置文件：" << file;
    ConfigPtr config = std::make_shared<Config>();
    if (config->LoadConfig(file))
    {
        std::lock_guard<std::mutex> lk(lock_);
        config_ = config;
        return true;
    }
    return false;
};
ConfigPtr ConfigMgr::GetConfig()
{
    std::lock_guard<std::mutex> lk(lock_);
    return config_;
};