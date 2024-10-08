
#pragma once
#include <string>
#include <cstdint>

namespace tmms
{
    namespace base
    {
        class TTime
        {

        public:
            static int16_t NowMs();
            static int16_t Now();
            static int16_t Now(int &year, int &month, int &day, int &hour, int &minute, int &second);
            static std::string ISOTime();
        };

    }
}