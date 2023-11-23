#pragma once
#include <cstdint>
#include "R.A.G.Engine/GFxLoaderDataState.hpp"
#include "R.A.G.Engine/GFxLoaderState.hpp"

namespace rage {
    class GFxLoaderData {
    public:
        char pad_0000[32]; //0x0000
        GFxLoaderState* m_state; //0x0020
        static GFxLoaderData* Get();
        static GFxLoaderData* GetData() {
            return GFxLoaderDataState::Get()->GetData(GFxLoaderDataState::Get());
        }
    }; //Size: 0x0028
    static_assert(sizeof(GFxLoaderData) == 0x28);
}