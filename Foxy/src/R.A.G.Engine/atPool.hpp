#pragma once
#include <cstdint>

namespace rage {
    class atPool {
    public:
        int8_t* m_data; //0x0000
        int8_t* m_flags; //0x0008
        uint32_t m_max; //0x0010
        uint32_t m_size; //0x0014
        uint32_t m_count; //0x0018

        template <typename T>
        T* GetAt(int index) {
            if (m_flags[index] < 0)
                return nullptr;
            return (T*)(m_data + (index * m_size));
        }
    }; //Size: 0x0020
    static_assert(sizeof(atPool) == 0x20);
}