/**
 * @Desc: 定时任务
 * @Author: zwp
 * @Date: 2024-09-27 09:54:55
 */
#pragma once
#include <cstdint>
#include <functional>
#include <memory>
namespace tmms
{
    namespace base
    {
        class Task;
        using TaskPtr = std::shared_ptr<Task>;
        using TaskCallback = std::function<void(const TaskPtr &)>;
        class Task : public std::enable_shared_from_this<Task>
        {
        private:
            /* data */
        public:
            Task(const TaskCallback &cb, int64_t interval);
            Task(const TaskCallback &&cb, int64_t interval);

            void Run();
            void Restart();

            int16_t When() const
            {
                return when_;
            }

        private:
            int16_t interval_{0};
            int16_t when_{0};
            TaskCallback cb_;
        };

    }
}
