#pragma once
#include <cstdint>
#include "R.A.G.Engine/atPool.hpp"

namespace rage {
    class strStreamingModule {
    public:
        uintptr_t m_vtable; //0x0000
        uint32_t m_base_index; //0x0008
        char pad_000C[40]; //0x000C //This might be a sub class, no idea.
        atPool m_pool; //0x0038
    }; //Size: 0x0058
    static_assert(sizeof(strStreamingModule) == 0x58);
}