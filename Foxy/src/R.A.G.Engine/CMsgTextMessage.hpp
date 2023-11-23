#pragma once
#include <cstdint>
#include "GamerHandle.hpp"

#pragma pack(push, 1)
class CMsgTextMessage {
public:
	char m_message[256]; //0x0000
	uint64_t m_peer_id; //0x0100
	bool m_is_team; //0x0108
}; //Size: 0x0109
static_assert(sizeof(CMsgTextMessage) == 0x109);
#pragma pack(pop)
namespace rage {
	class netTextMsg {
	public:
		char m_message[256]; //0x0000
		rage::rlGamerHandle m_handle; //0x0100
	}; //Size: 0x0110
	static_assert(sizeof(netTextMsg) == 0x110);
}
#pragma pack(push, 1)
class CMsgTextMessage2 {
public:
	char m_message[256]; //0x0000
	uint64_t m_peer_id; //0x0100
}; //Size: 0x0108
static_assert(sizeof(CMsgTextMessage2) == 0x108);
#pragma pack(pop)