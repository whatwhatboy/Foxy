#pragma once
#include <cstdint>
#include "GFxLoaderData.hpp"
#include "Memory/Patterns/Pointers.hpp"

namespace rage {
    class GFxLoader {
    public:
        GFxLoaderData* m_data; //0x0000
        static GFxLoader* Get() {
            return base::pointers::g_gfxLoader;
        }
    }; //Size: 0x0008
    static_assert(sizeof(GFxLoader) == 0x8);
}