#pragma once
#include <cstdint>

namespace rage {
	#pragma pack(push, 1)
	class JSONSerialiser {
	public:
		JSONSerialiser(char* buffer, uint32_t length) : m_buffer(buffer), m_max_length(length) {
			unk_0000 = 0;
			unk_0004 = 0;
			m_current_length = 0;
			unk_0018 = 1;
			m_flags = 0;
		}
	public:
		uint32_t unk_0000; //0x0000
		uint32_t unk_0004; //0x0004
		char* m_buffer; //0x0008
		uint32_t m_current_length; //0x0010
		uint32_t m_max_length; //0x0014
		uint32_t unk_0018; //0x0018
		uint8_t m_flags; //0x001C

		std::string get_buffer() {
			return m_buffer;
		}
	}; //Size: 0x0020
	static_assert(sizeof(JSONSerialiser) == 0x1D);
	#pragma pack(pop)
}