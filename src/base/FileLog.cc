
#include <string>
#include "FileLog.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
using namespace tmms::base;

bool FileLog::Open(const std::string &filePath)
{
    file_path_ = filePath;
    int fd = ::open(filePath.c_str(), O_CREAT | O_APPEND | O_WRONLY, DEFFILEMODE);
    if (fd < 0)
    {
        std::cout << "打开日志文件失败，路径：" << filePath << std::endl;
        return false;
    }
    fd_ = fd;
    return true;
};
size_t FileLog::WriteLog(const std::string &msg)
{
    int fd = fd_ == -1 ? 1 : fd_;
    return ::write(fd, msg.data(), msg.size());
};
void FileLog::Rotate(const std::string &file)
{
    if (file_path_.empty())
        return;
    // file_path_ = file;
    int ret = ::rename(file_path_.c_str(), file.c_str());
    if (ret != 0)
    {
        std::cerr << "重命名文件失败" << file_path_ << " : " << file;
        return;
    }
    int fd = ::open(file_path_.c_str(), O_CREAT | O_APPEND | O_WRONLY, DEFFILEMODE);
    if (fd < 0)
    {
        std::cout << "打开日志文件失败，路径：" << file << std::endl;
        return;
    }
    ::dup2(fd, fd_);
    close(fd);
};
void FileLog::SetRotate(RotateType type)
{
    rotateType_ = type;
};
RotateType FileLog::GetRotateTpe() const
{
    return rotateType_;
};
int64_t FileLog::FileSize()
{
    return ::lseek64(fd_, 0, SEEK_END);
};
std::string FileLog::FilePath() const
{
    return file_path_;
};
