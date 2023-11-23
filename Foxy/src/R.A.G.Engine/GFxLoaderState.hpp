#pragma once
#include <cstdint>

namespace rage {
    class GFxLoaderDataState;
    class GFxLoaderState {
    public:
        virtual GFxLoaderDataState* GetData(GFxLoaderState* state) = NULL;
        char pad_0000[480]; //0x0008
        uint64_t m_unk_value; //0x01E8
        static GFxLoaderState* Get();
    }; //Size: 0x01F0
    static_assert(sizeof(GFxLoaderState) == 0x1F0);
}