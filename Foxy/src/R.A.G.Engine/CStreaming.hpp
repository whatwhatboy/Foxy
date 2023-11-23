#pragma once
#include <cstdint>
#include "R.A.G.Engine/streamingListEntry.hpp"
#include "R.A.G.Engine/streamingDataEntry.hpp"
#include "R.A.G.Engine/strStreamingModule.hpp"
#include "R.A.G.Engine/strStreamingModuleMgr.hpp"
#include "Memory/Patterns/Pointers.hpp"

struct Asset {
    int Value;
    bool Valid() {
        return Value != -1;
    }
    void Set(Asset v) {
        Value = v.Value;
    }
    void Set(int v) {
        Value = v;
    }
};
class CStreaming {
public:
    void RequestObject(Asset index, rage::eStreamingFlag flag) {
        base::pointers::g_requestObject(this, index.Value, static_cast<int32_t>(flag));
    }
    static CStreaming* Get() {
        return base::pointers::g_streaming;
    }
    rage::streamingDataEntry* Entry(Asset asset) {
        if (asset.Valid())
            return &m_entries[asset.Value];
        return nullptr;
    }

    rage::streamingDataEntry* m_entries; //0x0000
    char pad_0008[16]; //0x0008
    int32_t m_num_entries; //0x0018
    int32_t unk_001C; //0x001C
    char pad_0020[64]; //0x0020
    rage::streamingListEntry* m_request_list_head; //0x0060
    rage::streamingListEntry* m_request_list_tail; //0x0068
    char pad_0070[328]; //0x0070
    rage::strStreamingModuleMgr m_module_mgr; //0x01B8
    int32_t m_num_pending_requests; //0x01E0
    int32_t m_num_pending_requests_unk; //0x01E4
    int32_t m_num_pending_requests_preious; //0x01E8
}; //Size: 0x01F0
static_assert(sizeof(CStreaming) == 0x1F0);