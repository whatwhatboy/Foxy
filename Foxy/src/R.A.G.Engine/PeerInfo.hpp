#pragma once
#include <cstdint>
#include "GamerHandle.hpp"
#include "NetSocketAddress.hpp"

namespace rage {
	#pragma pack(push, 8)
	class rlPeerInfo {
	public:
		char m_certificate[96]; //0x0000
		uint64_t m_peer_id; //0x0060
		class rage::rlGamerHandle m_gamer_handle; //0x0068
		char m_aes_key[32]; //0x0078
		uint8_t unk_0098; //0x0098
		class rage::netSocketAddress m_unk_address; //0x0099
		class rage::netSocketAddress m_public_address; //0x00A9
		class rage::netSocketAddress m_relay_address; //0x00A1
		class rage::netSocketAddress m_local_address;  //0x00B1
		uint32_t unk_00B9; //0x00B9
	}; //Size: 0x00C0
	static_assert(sizeof(rage::rlPeerInfo) == 0xC0);
	#pragma pack(pop)
}