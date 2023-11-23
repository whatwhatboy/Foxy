#pragma once
#include "Vectors.hpp"
#include "Enums.hpp"

namespace rage {
	#pragma pack(push, 1)
	class CBaseModelInfo {
	public:
		char pad_0000[24]; //0x0000
		uint32_t m_model; //0x0018
		char pad_001C[20]; //0x001C
		struct vector3 m_min_dimensions; //0x0030
		char pad_003C[4]; //0x003C
		struct vector3 m_max_dimensions; //0x0040
		char pad_004C[81]; //0x004C
		uint8_t m_model_type; //0x009D
		char pad_009E[6]; //0x009E
	}; //Size: 0x00A4
	static_assert(sizeof(CBaseModelInfo) == 0xA4);
	#pragma pack(pop)
}