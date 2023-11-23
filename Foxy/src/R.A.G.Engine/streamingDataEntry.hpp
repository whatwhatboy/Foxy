#pragma once
#include <cstdint>

namespace rage {
    enum class eStreamingFlag : uint32_t {
        EntryLoaded = 3,
        ReleaseObjectOnDeletion = 7,
        CreateMovieOnceLoaded = 50,
    };
    class streamingDataEntry {
    public:
        uint32_t m_handle; //0x0000
        uint32_t m_flags; //0x0004
        bool IsFlagSet(eStreamingFlag flag) {
            return (m_flags & (uint32_t)flag) != NULL;
        }
    }; //Size: 0x0008
    static_assert(sizeof(streamingDataEntry) == 0x8);
}