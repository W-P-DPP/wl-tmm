#include "TaksMgr.h"
#include <mutex>
#include "TTime.h"

using namespace tmms::base;

void TaksMgr::OnWork()
{
    std::lock_guard<std::mutex> lk(lock_);
    int16_t now = TTime::NowMs();
    for (auto iter = tasks_.begin(); iter != tasks_.end();)
    {
        if ((*iter)->When() < now)
        {
            (*iter)->Run();
            if ((*iter)->When() < now)
            {
                iter = tasks_.erase(iter);
                continue;
            }
        }
        iter++;
    }
};

bool TaksMgr::Add(TaskPtr &prt)
{
    std::lock_guard<std::mutex> lk(lock_);
    auto iter = tasks_.find(prt);
    if (iter != tasks_.end())
        return false;
    tasks_.emplace(prt);
    return true;
};
bool TaksMgr::Del(TaskPtr &prt)
{
    std::lock_guard<std::mutex> lk(lock_);
    tasks_.erase(prt);
    return true;
};
