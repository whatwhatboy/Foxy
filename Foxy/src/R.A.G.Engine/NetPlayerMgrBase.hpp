#pragma once
#include <cstdint>
#include "CNetGamePlayerDataMsg.hpp"
#include "CNetGamePlayer.hpp"

namespace rage {
	#pragma pack(push, 8)
	class netPlayerMgrBase {
	public:
		virtual ~netPlayerMgrBase() = default;
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		virtual void* AssignPhyiscalIndex(class CNetGamePlayer* netGamePlayer, uint8_t index) = 0;
		virtual CNetGamePlayer* RawAddPlayer(class rlGamerInfo* gamerInfo, uint32_t a2, class CNetGamePlayerDataMsg* playerData, class CNonPhysicalPlayerData* nonPhysicalPlayerData) = 0;
		virtual void RemovePlayer(class CNetGamePlayer* netGamePlayer) = 0;
		virtual void UpdatePlayerListsForPlayer(class CNetGamePlayer* netGamePlayer) = 0;
		virtual CNetGamePlayer* AddPlayer(class rlGamerInfo* gamerInfo, uint32_t a2, class CNetGamePlayerDataMsg* playerData, class CNonPhysicalPlayerData* nonPhysicalPlayerData) = 0;
	public:
		CNetGamePlayer* GetHost() {
			for (uint16_t i{ m_player_count }; i; --i) {
				if (m_player_list[i] && m_player_list[i]->IsConnected() && m_player_list[i]->IsNetworkHost())
					return m_player_list[i];
			}
			return nullptr;
		}
	public:
		char pad_0008[8]; //0x0008
		uint64_t* m_network_bandwidth_manager; //0x0010
		char pad_0018[216]; //0x0018
		class CNetGamePlayer* m_local_net_player; //0x00E8
		char pad_00F0[144]; //0x00F0
		class CNetGamePlayer* m_player_list[32]; //0x0180
		uint16_t m_player_limit; //0x0280
		char pad_0282[10]; //0x0282
		uint16_t m_player_count; //0x028C
		char pad_0290[1618]; //0x0290
	}; //Size: 0x08E0
	static_assert(sizeof(netPlayerMgrBase) == 0x8E8);
	#pragma pack(pop)
}