#pragma once
#include "FiberMgr.hpp"

namespace base {
    class fiberPool {
    public:
        void create();
    public:
        void queue(std::function<void()> func);
    public:
        static void fiberFunction();
        void tick();
    public:
        size_t m_numberOfFibers{ 10 };
        std::recursive_mutex m_mutex{};
        std::stack<std::function<void()>> m_jobs{};
    };
    inline fiberPool g_fiberPool{};
}