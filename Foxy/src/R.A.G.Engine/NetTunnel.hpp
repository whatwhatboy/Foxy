#pragma once
#include <cstdint>

namespace rage {
	class netTunnelDesc {
	public:
		netTunnelDesc(uint32_t type, uint32_t reason, bool unk) : m_type(type), m_reason(reason), m_connection_identifier(NULL), unk_0014(unk) {}
		netTunnelDesc(uint32_t type, uint32_t reason) : netTunnelDesc(type, reason, false) {}
		netTunnelDesc() : netTunnelDesc(NULL, NULL) {}
		uint32_t m_type; //0x0000
		uint32_t m_reason; //0x0004
		uint64_t m_connection_identifier; //0x000C
		bool unk_0014; //0x0014
	}; //Size: 0x0015
	class netTunnelRequest {
	public:
		uint32_t m_identifier; //0x0000
		int32_t m_state; //0x0004
		uint64_t unk_0008; //0x0008
		uint32_t m_connection_identifier; //0x0010
		char pad_0014[4096]; //0x0014
	}; //Size: 0x1014
	class netStatus {
	public:
		uint32_t m_status_code; //0x0000
		uint32_t unk_0004; //0x0004
	}; //Size: 0x0008
}