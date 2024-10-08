

/**
 * @Desc: 单例
 * @Author: zwp
 * @Date: 2024-09-29 10:15:34
 */
#pragma once

#include "NonCopyable.h"
#include <pthread.h>

namespace tmms
{
    namespace base
    {

        template <typename T>
        class Singeton : public NonCopyable
        {
        private:
            static void init()
            {
                if (!value_)
                {
                    value_ = new T();
                }
            };
            static pthread_once_t ponce_;
            static T *value_;

        public:
            Singeton() = delete;
            ~Singeton() = delete;

            static T *&Instance()
            {
                pthread_once(&ponce_, &Singeton::init);
                return value_;
            };
        };

        template <typename T>
        pthread_once_t Singeton<T>::ponce_ = PTHREAD_ONCE_INIT;

        template <typename T>
        T *Singeton<T>::value_ = nullptr; // c++ nullptr 表示空指针
    }
}
