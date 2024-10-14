/**
 * @Desc: 定时任务管理器
 * @Author: zwp
 * @Date: 2024-09-27 11:01:48
 */
#pragma once
#include "Singeton.h"
#include "Task.h"
#include <unordered_set>
#include <mutex>

namespace tmms
{
    namespace base
    {
        class TaksMgr : NonCopyable
        {
        private:
        public:
            TaksMgr() = default;
            ~TaksMgr() = default;

            void OnWork();

            bool Add(TaskPtr &prt);
            bool Del(TaskPtr &prt);

        private:
            std::unordered_set<TaskPtr> tasks_;
            std::mutex lock_;
            // std::defer_lock_t lock_;
        };
    }
#define sTaskMgr tmms::base::Singeton<tmms::base::TaksMgr>::Instance()
}