
/**
 * @Desc: 不可复制或操作的类
 * @Author: zwp
 * @Date: 2024-09-19 16:48:02
 */
#pragma once

namespace tmms
{
    namespace base
    {
        class NonCopyable
        {
        protected:
            NonCopyable() {};
            ~NonCopyable() {};
            NonCopyable(const NonCopyable &) = delete;
            NonCopyable &operator=(const NonCopyable &) = delete;
        };
    }
}