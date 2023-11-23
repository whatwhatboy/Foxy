#pragma once
#include <cstdint>
#include "PeerInfo.hpp"
#include "GamerHandle.hpp"

namespace rage {
	#pragma pack(push, 8)
	class rlGamerInfo : public rlPeerInfo {
	public:
		uint64_t m_peer_address; //0x00C0
		union {
			uint64_t m_platform_data; //0x00C8
			rlGamerHandle m_unk_gamer_handle; //0x00C8
		};
		uint32_t m_ros_privilege; //0x00D0
		char m_name[20]; //0x00D4
	}; //Size: 0x00F0
	static_assert(sizeof(rlGamerInfo) == 0xF0);
	#pragma pack(pop)
}