#pragma once
#include <cstdint>

namespace rage {
	class streamingListEntry {
	public:
		streamingListEntry* m_prev; //0x0000
		streamingListEntry* m_next; //0x0008
		uint32_t m_index; //0x0010
	}; //Size: 0x0018
}