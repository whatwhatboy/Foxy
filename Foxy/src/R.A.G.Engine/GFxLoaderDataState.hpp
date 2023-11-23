#pragma once
#include "R.A.G.Engine/GFxLoaderState.hpp"

namespace rage {
    class GFxLoaderDataState {
    public:
        virtual class GFxLoaderData* GetData(GFxLoaderDataState* loader) = NULL;
        static GFxLoaderDataState* Get() {
            return GFxLoaderState::Get()->GetData(GFxLoaderState::Get());
        }
    }; //Size: 0x0008
    static_assert(sizeof(GFxLoaderDataState) == 0x8);
}