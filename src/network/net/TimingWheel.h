#pragma once

#include <memory>
#include <functional>
#include <cstdint>
#include <vector>
#include <unordered_set>
#include <deque>
namespace tmms
{
    namespace network
    {
        using Func = std::function<void()>;
        using Wheels = std::vector<Wheel>;
        using Wheel = std::deque<WheelEntry>;
        using WheelEntry = std::unordered_set<EntryPtr>;
        using EntryPtr = std::shared_ptr<void>;

        const int kTimingMinute = 60;
        const int kTimingHour = 60 * 60;
        const int kTimingDay = 60 * 60 * 24;

        enum TImingType
        {
            kTimingTypeSecond = 0,
            kTimingTypeMinute = 1,
            kTimingTypeHour = 2,
            kTimingTypeDay = 3,

        };

        class CallBackEntry
        {
        public:
            CallBackEntry(const Func &cb) : cb_(cb) {};
            ~CallBackEntry()
            {
                if (cb_)
                {
                    cb_();
                }
            };

        private:
            Func cb_;
        };
        using CallBackEntryPtr = std::shared_ptr<CallBackEntry>;

        class TimingWheel
        {
        private:
            Wheels Wheels_;
            int64_t last_ts_{0};
            uint64_t tick_{0};

            void InsertSecondEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertMinuteEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertHourEntry(uint32_t delay, EntryPtr entryPtr);
            void InsertDayEntry(uint32_t delay, EntryPtr entryPtr);

        public:
            TimingWheel(/* args */);
            ~TimingWheel();

            void InsertEntry(uint32_t delay, EntryPtr entryPtr);
            void OnTimer(int64_t now);
            void PopUp(Wheel &bq);
            void RunAfter(double delay, const Func &cb);
            void RunAfter(double delay, Func &&cb);
            void RunEvery(double interval, const Func &cb);
            void RunEvery(double interval, Func &&cb);
        };

    }
}