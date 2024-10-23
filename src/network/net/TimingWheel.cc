#include <TimingWheel.h>
#include "network/base/Network.h"
using namespace tmms::network;

TimingWheel::TimingWheel() : Wheels_(4)
{
    Wheels_[kTimingTypeSecond].resize(60);
    Wheels_[kTimingTypeMinute].resize(60);
    Wheels_[kTimingTypeHour].resize(24);
    Wheels_[kTimingTypeDay].resize(30);
};
TimingWheel::~TimingWheel() {};

void TimingWheel::InsertSecondEntry(uint32_t delay, EntryPtr entryPtr)
{
    Wheels_[kTimingTypeSecond][delay - 1].emplace(entryPtr);
};
void TimingWheel::InsertMinuteEntry(uint32_t delay, EntryPtr entryPtr)
{
    auto minute = delay / 60;
    auto second = delay % 60;
    CallBackEntryPtr newEntry = std::make_shared<CallBackEntry>([this, second, entryPtr]()
                                                                { InsertEntry(second, entryPtr); });
    Wheels_[kTimingTypeMinute][minute - 1].emplace(newEntry);
};
void TimingWheel::InsertHourEntry(uint32_t delay, EntryPtr entryPtr)
{
    auto hour = delay / kTimingHour;
    auto second = delay % kTimingHour;
    CallBackEntryPtr newEntry = std::make_shared<CallBackEntry>([this, hour, entryPtr]()
                                                                { InsertEntry(hour, entryPtr); });
    Wheels_[kTimingTypeHour][hour - 1].emplace(newEntry);
};
void TimingWheel::InsertDayEntry(uint32_t delay, EntryPtr entryPtr)
{
    auto day = delay / kTimingDay;
    auto second = delay % kTimingDay;
    CallBackEntryPtr newEntry = std::make_shared<CallBackEntry>([this, day, entryPtr]()
                                                                { InsertEntry(day, entryPtr); });
    Wheels_[kTimingTypeDay][day - 1].emplace(newEntry);
};

void TimingWheel::InsertEntry(uint32_t delay, EntryPtr entryPtr)
{
    if (delay <= 0)
    {
        entryPtr.reset();
    }
    if (delay < kTimingMinute)
    {
        InsertSecondEntry(delay, entryPtr);
    }
    else if (delay < kTimingHour)
    {
        InsertMinuteEntry(delay, entryPtr);
    }
    else if (delay < kTimingDay)
    {
        InsertHourEntry(delay, entryPtr);
    }
    else
    {
        auto day = delay / kTimingDay;
        if (day > 30)
        {
            NETWORK_ERROR << "不支持大于30天";
            return;
        }
        InsertDayEntry(delay, entryPtr);
    }
};
void TimingWheel::OnTimer(int64_t now)
{
    if (last_ts_ == 0)
    {
        last_ts_ = now;
    }
    if (now - last_ts_ < 1000)
    {
        return;
    }

    last_ts_ = now;
    ++tick_;
    PopUp(Wheels_[kTimingTypeSecond]);
    if (tick_ % kTimingMinute == 0)
    {
        PopUp(Wheels_[kTimingTypeMinute]);
    }
    else if (tick_ % kTimingHour == 0)
    {
        PopUp(Wheels_[kTimingTypeHour]);
    }
    else if (tick_ % kTimingDay == 0)
    {
        PopUp(Wheels_[kTimingTypeDay]);
    }
};
void TimingWheel::PopUp(Wheel &bq)
{
    bq.front().clear();
    bq.pop_front();
    bq.push_back(WheelEntry());
};
void TimingWheel::RunAfter(double delay, const Func &cb)
{
    CallBackEntryPtr cbEntry = std::make_shared<CallBackEntry>([cb]()
                                                               { cb(); });
    InsertEntry(delay, cbEntry);
};
void TimingWheel::RunAfter(double delay, Func &&cb)
{
    CallBackEntryPtr cbEntry = std::make_shared<CallBackEntry>([cb]()
                                                               { cb(); });
    InsertEntry(delay, cbEntry);
};
void TimingWheel::RunEvery(double interval, const Func &cb)
{
    CallBackEntryPtr cbEntry = std::make_shared<CallBackEntry>([this, cb, interval]()
                                                               {
        cb();
        RunEvery(interval,cb); });
    InsertEntry(interval, cbEntry);
};
void TimingWheel::RunEvery(double interval, Func &&cb)
{

    CallBackEntryPtr cbEntry = std::make_shared<CallBackEntry>([this, cb, interval]()
                                                               {
        cb();
        RunEvery(interval,cb); });
    InsertEntry(interval, cbEntry);
};