#pragma once
#include <cstdint>
#include "GamerInfo.hpp"

#pragma pack(push, 8)
class RemoteGamerInfoMsg
{
public:
	uint64_t m_session_id; //0x0000
	class rage::rlGamerInfo m_gamer_info; //0x0008
	class rage::netConnectionPeer m_gamer_peer_data; //0x00A0
	uint32_t unk_00C0; //0x00C0
	uint32_t m_unk_struct_size; //0x00C4 
	char m_unk_struct[512]; //0x00C8 Might be bitbuffer data
	uint32_t m_num_handles; //0x02C8
	class rage::rlGamerHandle m_handles[32]; //0x02D0
}; //Size: 0x04D0
static_assert(sizeof(RemoteGamerInfoMsg) == 0x528);
#pragma pack(pop)