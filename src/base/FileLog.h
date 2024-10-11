

#pragma once
#include <string>
#include <memory>
namespace tmms
{
    namespace base
    {
        enum RotateType
        {
            kRotateNone,
            kRotateMinute,
            kRotateHour,
            kRotateDay,
        };
        class FileLog
        {
        private:
            int16_t fd_{-1};
            std::string file_path_;
            RotateType rotateType_{kRotateNone};

        public:
            FileLog() = default;
            ~FileLog() = default;

            bool Open(const std::string &filePath);
            size_t WriteLog(const std::string &msg);
            void Rotate(const std::string &file);
            void SetRotate(RotateType type);
            RotateType GetRotateTpe() const;
            int64_t FileSize();
            std::string FilePath() const;
        };
        using FileLogPtr = std::shared_ptr<FileLog>;

    }

}