#pragma once
#include <cstdint>

namespace rage {
	#pragma pack(push, 4)
	class fwDrawData {
	public:
		uint64_t unk_000; //0x0000
		uint64_t unk_008; //0x0008
		char pad_0010[8]; //0x0010
		uint32_t dword18; //0x0018
		uint32_t unk_001C; //0x0028
		uint64_t unk_0020; //0x0028
		uint32_t unk_0028; //0x0028
	}; //Size: 0x002C
	static_assert(sizeof(fwDrawData) == 0x2C);
	#pragma pack(pop)
}