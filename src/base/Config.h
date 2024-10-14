#pragma once
#include <string>
#include "jsoncpp/json/json.h"
#include <memory>
#include <cstdint>
#include "NonCopyable.h"
#include <mutex>
#include "Singeton.h"
#include "Logger.h"
#include "FileLog.h"
namespace tmms
{
    namespace base
    {
        struct LogInfo
        {
            LoggerLevel level;
            std::string path;
            std::string name;
            RotateType rotate_type{kRotateNone};
        };
        using LogInfoPtr = std::shared_ptr<LogInfo>;
        class Config
        {
        private:
            LogInfoPtr log_info_;
            bool ParseLogInfo(const Json::Value &root);

        public:
            Config() = default;
            ~Config() = default;

            bool LoadConfig(const std::string &file);

            LogInfoPtr &GetLogInfo();

            std::string name_;
            int32_t cpu_start_{0};
            int32_t thread_num_{1};
        };
        using ConfigPtr = std::shared_ptr<Config>;

        class ConfigMgr : public NonCopyable
        {
        private:
            ConfigPtr config_;
            std::mutex lock_;

        public:
            ConfigMgr(/* args */) = default;
            ~ConfigMgr() = default;

            bool LoadConfig(const std::string &file);
            ConfigPtr GetConfig();
        };
#define sConfigMgr tmms::base::Singeton<tmms::base::ConfigMgr>::Instance()
    }
}
