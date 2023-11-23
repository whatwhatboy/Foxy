#pragma once
#include "pgStreamableBase.hpp"

namespace rage {
	struct BlockInfo {
		uint32_t offset; //0x0000
		void* data; //0x0004
		uint32_t size; //0x000C
	}; //Size: 0x0018
	static_assert(sizeof(BlockInfo) == 0x18);
	struct BlockMap {
		inline BlockMap() {
			m_virtual_len = 0;
			m_physical_len = 0;
			memset(m_blocks, NULL, sizeof(m_blocks));
			*(uint16_t*)(uint64_t(this) + 0x8) = 0x407;
		}
		uint16_t m_virtual_len; //0x0000
		uint16_t m_physical_len; //0x0002
		BlockInfo m_blocks[128]; //0x0004
		size_t m_base_allocation_size[2]; //0x0C08
	}; //Size: 0x0C18
	static_assert(sizeof(BlockMap) == 0xC18);
}