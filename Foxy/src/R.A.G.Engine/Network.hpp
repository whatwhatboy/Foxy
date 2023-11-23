#pragma once
#include "Pch/Common.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "NetSocketAddress.hpp"
#include "CNetGamePlayer.hpp"
#include "NetPlayer.hpp"
#include "rlMetric.hpp"
#include "SessionInfo.hpp"
#include "GamerInfo.hpp"
#include "NetPlayerMgrBase.hpp"
#include "CNetworkPlayerMgr.hpp"
#include "SysMemAllocator.hpp"
#include "GamerHandle.hpp"
#include "SessionInfo.hpp"
#include "BitBuffer.hpp"

namespace rage {
	template <typename t>
	class ObfVar {
	public:
		t getData() {
			auto v105 = m_unk4;
			auto v28 = m_unk1 & v105;
			auto v94 = m_unk2 & ~v105;
			return v28 | v94;
		}
		operator t() {
			return getData();
		}
		#if _WIN32
		void setData(t val) {
			auto seed = time(nullptr);
			m_unk3 = seed;
			seed = time(nullptr);
			m_unk4 = seed;

			auto v48 = val & ~seed;
			m_unk1 = seed & val;
			m_unk2 = v48;
		}
		void operator =(t val) {
			setData(val);
		}
		#endif
	private:
		t m_unk1;
		t m_unk2;
		t m_unk3;
		t m_unk4;
	};
	using Obf16 = ObfVar<unsigned short>;
	using Obf32 = ObfVar<unsigned int>;
	#pragma pack(push, 4)
	class netConnectionManager {
	public:
		char pad_0000[216]; //0x0000
		uint64_t unk_00E8; //0x00D8
		uint32_t unk_00E0; //0x00E0
		char pad_00E4[8]; //0x00E4
		uint8_t unk_00EC; //0x00EC
		uint32_t unk_00F0; //0x00F0
	}; //Size: 0x00F4
	static_assert(sizeof(netConnectionManager) == 0xF4);
	#pragma pack(pop)
	class rlSessionByGamerTaskResult {
	public:
		rlGamerHandle m_gamer_handle{ 0 }; //0x0000
		rlSessionInfo m_session_info{}; //0x0010
	}; //Size: 0x00E0
	static_assert(sizeof(rlSessionByGamerTaskResult) == 0xE0);
	class rlPresenceEventInviteAccepted {
	public:
		rlPresenceEventInviteAccepted(rlGamerInfo& gamerInfo, rlSessionInfo& sessionInfo, rlGamerHandle& gamerHandle) : m_gamer_info(gamerInfo), m_session_info(sessionInfo), m_gamer_handle(gamerHandle) {
			m_self = this;
		}
		virtual ~rlPresenceEventInviteAccepted() {}
		virtual void unk_0008() {}

		rlPresenceEventInviteAccepted* m_self; //0x0008
		rlGamerInfo m_gamer_info; //0x0010
		char pad_0100[8]; //0x0100
		rlSessionInfo m_session_info; //0x0108
		rlGamerHandle m_gamer_handle; //0x01D8
	}; //Size: 0x01E8
	static_assert(sizeof(rlPresenceEventInviteAccepted) == 0x1E8);
	#pragma pack(push, 1)
	class snPlayer {
	public:
		uint64_t m_msg_id; //0x0000
		class rage::rlGamerInfo m_gamer_info; //0x0008
		char pad_00F8[8]; //0x00F8
	}; //Size: 0x00100
	static_assert(sizeof(rage::snPlayer) == 0x100);
	#pragma pack(pop)
	#pragma pack(push, 1)
	class snPeer {
	public:
		class rage::rlGamerInfo m_gamer_data; //0x0000
		char pad_0098[40]; //0x0098
	}; //Size: 0x0118
	static_assert(sizeof(rage::snPeer) == 0x118);
	#pragma pack(push, 1)
	class rlRemoteGamer {
	public:
		rage::rlGamerHandle m_handle;
		char pad_0010[4]; //0x0010
		uint32_t m_timeout_time; //0x0014
		uint32_t m_time_unk; //0x0018
		char pad_001C[4]; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(rage::rlRemoteGamer) == 0x20);
	#pragma pack(pop)
	#pragma pack(push, 1)
	class rlSession {
	public:
		virtual ~rlSession() = default;
		
		char pad_0008[248]; //0x0008
		class rage::rlSessionInfo m_session_info; //0x0100
		char pad_01D0[296]; //0x01D0
		uint64_t m_session_id; //0x02F8
		char pad_0300[1136]; //0x0300
	}; //Size: 0x770
	static_assert(sizeof(rage::rlSession) == 0x770);
	#pragma pack(pop)
	#pragma pack(push, 1)
	class rlSessionDetail {
	public:
		class rage::rlPeerInfo m_peer_info;
		char pad_0060[8]; //0x0060
		class rage::rlSessionInfo m_session_info; //0x0068
		char pad_00D8[14]; //0x00D8
		uint16_t m_session_type; //0x00E6
		char pad_00E8[324]; //0x00E8
		uint32_t m_player_count; //0x022C
		uint32_t m_unk_player_count; //0x0230
		char pad_0234[2]; //0x0234
		int16_t m_unk_pos_x; //0x0236
		int16_t m_unk_pos_y; //0x0238
		int16_t m_unk_pos_z; //0x023A
		uint8_t m_matchmaking_property_ids[32]; //0x023C
		char pad_025C[2]; //0x025C
		uint16_t m_rank; //0x025E
		char pad_0260[1]; //0x0260
		uint8_t m_mental_state; //0x0261
		char pad_0262[21]; //0x0262
		uint8_t m_population_density; //0x0277
		char pad_0278[320]; //0x0278
	}; //Size: 0x03CA
	static_assert(sizeof(rlSessionDetail) == 0x478);
	#pragma pack(pop)
	#pragma pack(push, 1)
	class rlMatchmakingFindResult {
	public:
		class rage::rlSessionDetail m_result_session_details[15]; //0x0000
		char pad_4308[168]; //0x4308
	}; //Size: 0x43B0
	static_assert(sizeof(rage::rlMatchmakingFindResult) == 0x43B0);
	#pragma pack(push, 1)
	class netGamePlayerData {
	public:
		class rlGamerHandle m_handle; //0x0000
		bool m_is_activity_spectator; //0x0010
		char pad_0011[7]; //0x0011
		uint64_t m_crew_id; //0x0018
		uint16_t m_rank; //0x0020
		uint16_t m_debug_unk; //0x0022
		char pad_0024[4]; //0x0024
		uint32_t m_nat_type; //0x0028
		bool m_is_rockstar_dev; //0x002C
		char pad_002D[3]; //0x002D
	}; //Size: 0x0030
	static_assert(sizeof(rage::netGamePlayerData) == 0x30);
	#pragma pack(pop)
	class netConnectionManager;
	#pragma pack(push, 8)
	class netConnectionPeer {
	public:
		netSocketAddress m_internal_address; //0x0000
		netSocketAddress m_external_address; //0x0008
		uint64_t m_peer_id; //0x0010
		uint32_t unk_0018; //0x0018
		uint16_t unk_001C; //0x001C
		uint8_t m_platform; //0x001E
	}; //Size: 0x0020
	static_assert(sizeof(netConnectionPeer) == 0x20);
	#pragma pack(pop)
	namespace netConnection {
		#pragma pack(push, 1)
		class InFrame {
		public:
			virtual ~InFrame() = default;
			virtual void Destroy() = 0;
			virtual uint32_t GetId() = 0;
			virtual uint32_t GetPayloadSize() = 0;
			virtual bool IsChanneled() = 0;

			uint32_t m_timestamp; //0x0008
			char pad_0008[52]; //0x000C
			uint32_t m_msg_id; //0x0040
			uint32_t m_connection_identifier; //0x0044
			InFrame* m_this; //0x0048
			uint32_t m_platform_data; //0x0050
			char pad_0054[4]; //0x054
			netConnectionPeer m_peer; //0x0058
			char pad_0070[8]; //0x0084
			uint32_t m_length; //0x0080
			char pad_007C[4]; //0x0084
			void* m_data; //0x0088
		}; //Size: 0x0090
		static_assert(sizeof(InFrame) == 0x90);
		#pragma pack(pop)
	}
	#pragma pack(push, 1)
	class snSession {
	public:
		uint64_t m_memory_allocator; //0x0000
		char pad_0008[64]; //0x0008
		rage::netConnectionManager* m_net_connection_mgr; //0x0048
		char pad_0050[48]; //0x0050
		class rage::rlSession m_rline_session; //0x0080
		class rage::snPlayer m_local_player; //0x07F0
		uint64_t m_host_token; //0x08F0
		char pad_08F8[144]; //0x08F8
		class rage::snPeer m_peer_storage[32]; //0x0988
		char pad_2C88[24]; //0x2C88
		class rage::snPeer* m_peers[32]; //0x2CA0
		uint32_t m_peer_count; //0x2DA0
		char pad_2DA4[4]; //0x2DA4
		class rage::snPlayer m_player_storage[32]; //0x2DA8
		char pad_4DA8[24]; //0x4DA8
		class rage::snPlayer* m_players[32]; //0x4DC0
		int32_t m_player_count; //0x4EC0
		char pad_4EC4[4]; //0x4EC4
		class rage::rlRemoteGamer m_remote_gamers[32]; //0x4EC8
		uint32_t m_num_remote_gamers; //0x52C8
		bool m_player_joining; //0x52CC
		char pad_52CD[107]; //0x52CD
		uint32_t m_connection_identifier; //0x5338
		char pad_533C[4]; //0x533C
		uint32_t m_profile_index; //0x5340
		char m_token_key[64]; //0x5344
		char m_id_key[64]; //0x5384
		char m_info_key[64]; //0x53C4
		char m_host_key[64]; //0x5404
		char m_join_key[64]; //0x5444
		char pad_5484[4]; //0x5484

		rage::snPlayer* getPlayerByPeerAddress(uint64_t peerAddress) {
			for (uint32_t i{}; i != m_player_count; ++i) {
				if (auto snPlayer = m_players[i]; snPlayer) {
					if (snPlayer->m_gamer_info.m_peer_address == peerAddress)
						return snPlayer;
				}
			}
			return nullptr;
		}
		rage::snPlayer* getPlayerByPlatformData(uint64_t platformData) {
			for (uint32_t i{}; i != m_player_count; ++i) {
				if (auto snPlayer = m_players[i]; snPlayer) {
					if (snPlayer->m_gamer_info.m_platform_data == platformData)
						return snPlayer;
				}
			}
			return nullptr;
		}
		rage::snPlayer* getPlayerByMsgId(uint32_t msgId) {
			for (uint32_t i{}; i != m_player_count; ++i) {
				if (auto snPlayer = m_players[i]; snPlayer) {
					if (snPlayer->m_msg_id == msgId)
						return snPlayer;
				}
			}
			return nullptr;
		}
		bool is_host() {
			return m_local_player.m_gamer_info.m_peer_address == m_host_token;
		}
		auto encoded_info() {
			return m_rline_session.m_session_info.encode();
		}
		auto session_info() {
			return m_rline_session.m_session_info;
		}
		auto session_id() {
			return m_rline_session.m_session_id;
		}

	}; //Size: 0x3E70
	static_assert(sizeof(rage::snSession) == 0x5488);
	#pragma pack(pop)
	class snMsgRemoveGamersFromSessionCmd {
	public:
		uint64_t m_session_id; //0x0000
		rage::rlGamerHandle m_handles[32]; //0x0008
		int32_t m_unk = -1; //0x208
		uint32_t m_num_handles; //0x20C
	}; //Size: 0x0110
	static_assert(sizeof(rage::snMsgRemoveGamersFromSessionCmd) == 0x210);
	class snMsgAddGamerToSessionCmd {
	public:
		uint64_t m_session_id; //0x0000
		class rlGamerInfo m_gamer_info; //0x0008
		class netConnectionPeer m_net_connection_peer; //0x00A0
		uint32_t unk_00C0; //0x00C0
		uint32_t m_unk_struct_size; //0x00C4
		char m_unk_struct[512]; //0x00C8
		uint32_t m_num_handles; //0x02C8
		rlGamerHandle m_handles[32]; //0x02C8
	}; //Size: 0x00A8
}

#pragma pack(push, 1)
class MetricSessionMigrated : public rage::rlMetric {
public:
    char pad_0008[828]; //0x0008
    uint32_t m_num_players; //0x0344
}; //Size: 0x0348
static_assert(sizeof(MetricSessionMigrated) == 0x348);
class NetworkGameConfig {
public:
	char pad_0000[48]; //0x0000
	uint32_t m_public_slots; //0x0030
	uint32_t m_private_slots; //0x0034
	char pad_0038[272]; //0x0038
}; //Size: 0x0148
static_assert(sizeof(NetworkGameConfig) == 0x148);
class NetworkGameFilterMatchmakingComponent {
public:
	uint32_t m_filter_type; //0x0000
	char m_filter_name[24]; //0x0004
	uint32_t m_num_parameters; //0x001C
	uint16_t m_game_mode; //0x0020
	uint16_t m_session_type; //0x0022
	uint32_t m_param_unk[8]; //0x0024
	char m_param_names[8][24]; //0x0044
	char pad_0104[4]; //0x0104
	uint32_t m_param_mappings[8]; //0x0108
	char pad_0128[352]; //0x0128
	uint32_t m_param_values[8]; //0x0288
	char pad_02A8[96]; //0x02A8
	uint32_t m_enabled_params_bitset; //0x0308
	char pad_030C[8]; //0x030C

	//Do not use for actual network filters, this will break things
	void SetParameter(const char* name, uint32_t index, uint32_t value) {
		std::strcpy(m_param_names[index], name);
		m_param_mappings[index] = index;
		m_param_values[index] = value;
		m_enabled_params_bitset |= (1 << index);
		if (m_num_parameters <= index)
			m_num_parameters++;
	}
}; //Size: 0x0314
static_assert(sizeof(NetworkGameFilterMatchmakingComponent) == 0x314);
class MatchmakingAttributes {
public:
	uint32_t m_game_mode; //0x0000
	uint32_t m_num_params; //0x0004
	uint32_t m_param_unk[8]; //0x0008
	char m_param_names[8][24]; //0x0028
	uint32_t m_param_values[8]; //0x00E8
	uint32_t m_params_bitset; //0x0108
}; //Size: 0x010C
static_assert(sizeof(MatchmakingAttributes) == 0x10C);
class NetworkGameFilter {
public:
	virtual ~NetworkGameFilter() = default;
	virtual void Reset() {};
public:
	uint32_t m_build_type; //0x0008
	uint32_t m_discriminator; //0x000C
	uint32_t m_discriminator_mapping; //0x0010
	uint32_t m_region_mapping; //0x0014
	uint32_t m_language_mapping; //0x0018
	uint32_t m_mm_group_1_mapping; //0x001C
	uint32_t m_mm_group_2_mapping; //0x0020
	uint32_t m_activity_type_mapping; //0x0024
	uint32_t m_activity_id_mapping; //0x0028
	uint32_t m_activity_players_mapping; //0x002C
	class NetworkGameFilterMatchmakingComponent m_matchmaking_component; //0x0030
}; //Size: 0x0344
static_assert(sizeof(NetworkGameFilter) == 0x344);
class SessionInfoBackup {
public:
	class rage::rlSessionInfo m_session_info; //0x0000
	uint32_t m_unk; //0x00D0
	char pad_0074[4]; //0x00D4
	uint32_t m_flags; //0x00D8
}; //Size: 0x00DC
static_assert(sizeof(SessionInfoBackup) == 0xDC);
class MatchmakingSessionResult {
public:
	class rage::rlSessionDetail m_detail; //0x0000
	char pad_0478[24]; //0x0478
}; //Size: 0x0490
static_assert(sizeof(MatchmakingSessionResult) == 0x490);
class CNetRemoteComplaint {
public:
	uint64_t m_complainer_token; //0x0000
	uint64_t m_complainee_token; //0x0008
	uint32_t m_flags; //0x0010
	uint32_t m_time; //0x0014
}; //Size: 0x0018
static_assert(sizeof(CNetRemoteComplaint) == 0x18);
class CNetComplaintMgr {
public:
	uint64_t m_peer_address; //0x0000
	uint32_t m_host_peer_id; //0x0008
	char pad_000C[4]; //0x000C
	class rage::netConnectionManager* m_net_connection_mgr; //0x0010
	char pad_0018[64]; //0x0018
	class rage::rlGamerHandle m_handles_in_scope[64]; //0x0058
	uint32_t m_num_handles_in_scope; //0x0458
	char pad_045C[4]; //0x045C
	class CNetRemoteComplaint m_remote_complaints[64]; //0x0460
	uint32_t m_num_remote_complaints; //0x0A60
	char pad_0A64[4]; //0x0A64
	uint64_t m_peer_addresses_complained[64]; //0x0A68
	uint32_t m_num_addresses_complained; //0x0C68
	char pad_0C6C[520]; //0x0C6C
	uint32_t m_connection_identifier; //0x0E74
	uint32_t m_last_resend_time; //0x0E78
	char pad_0E7C[4]; //0x0E7C
	uint32_t m_time_to_resend; //0x0E80
	uint32_t m_flags; //0x0E84
	char pad_0E88[16]; //0x0E88

	inline bool has_local_complaint(uint64_t peer_address) {
		for (uint32_t i{}; i != m_num_addresses_complained; ++i)
			if (m_peer_addresses_complained[i] == peer_address)
				return true;
		return false;
	}
	inline void raise_complaint(uint64_t peer_address, bool use_all_addresses_in_session, bool spoof_address) {
		auto net_mgr = (*base::pointers::g_networkPlayerMgr);
		if (has_local_complaint(peer_address) || peer_address == net_mgr->m_local_net_player->GetGamerInfo()->m_peer_address)
			return;
		auto old_token = m_peer_address;
		if (m_peer_address == old_token && spoof_address)
			m_peer_address = peer_address;
		if (use_all_addresses_in_session) {
			for (auto player : net_mgr->m_player_list) {
				if (auto gamer_info = player->GetGamerInfo(); player && player->IsConnected()) {
					if (gamer_info->m_peer_address != peer_address)
						m_peer_addresses_complained[m_num_addresses_complained++] = gamer_info->m_peer_address;
				}
			}
		}
		m_peer_addresses_complained[m_num_addresses_complained++] = peer_address;
		if (m_peer_address != old_token && spoof_address)
			m_peer_address = old_token;
	}
	inline uint32_t num_of_addresses_complainted() {
		return m_num_addresses_complained;
	}
	inline void remove_complaint(uint64_t peer_address) {
		if (!has_local_complaint(peer_address))
			return;
		for (uint32_t i{}; i != m_num_addresses_complained; ++i)
			if (m_peer_addresses_complained[i] == peer_address)
				m_peer_addresses_complained[i] = m_peer_addresses_complained[m_num_addresses_complained - 1];
		m_num_addresses_complained--;
	}
	inline void remove_all_complaints() {
		for (uint32_t i{}; i != m_num_addresses_complained; ++i)
			m_peer_addresses_complained[i] = m_peer_addresses_complained[m_num_addresses_complained - 1];
		m_num_addresses_complained = 0;
		base::pointers::g_resetNetworkComplaints(this);
	}
}; //Size: 0x0C98
static_assert(sizeof(CNetComplaintMgr) == 0xE98);
class PlayerNameMapNode {
public:
	char m_name[16]; //0x0000
	class rage::rlGamerHandle m_handle; //0x0010
	class PlayerNameMapNode* m_next; //0x0020
	class PlayerNameMapNode* m_prev; //0x0028
}; //Size: 0x0030
static_assert(sizeof(PlayerNameMapNode) == 0x30);
class JoiningPlayerNameMap {
public:
	class PlayerNameMapNode m_names[100]; //0x0000
	char pad_12C0[40]; //0x12C0
	uint32_t m_num_name_nodes; //0x12E8
	char pad_12EC[796]; //0x12EC
}; //Size: 0x1608
static_assert(sizeof(JoiningPlayerNameMap) == 0x1608);
class CNetBlacklistNode : public rage::rlGamerHandle {
public:
	bool m_block_rejoin; //0x0010
	char pad_0011[3]; //0x0011
	uint32_t m_added_time; //0x0014
	class CNetBlacklistNode* m_next; //0x0018
	class CNetBlacklistNode* m_prev; //0x0020
}; //Size: 0x0028
static_assert(sizeof(CNetBlacklistNode) == 0x28);
class CNetBlacklist {
public:
	class CNetBlacklistNode m_nodes[16]; //0x0000
	class CNetBlacklistNode* m_head; //0x0280
	class CNetBlacklistNode* m_tail; //0x0288
	uint32_t m_free_nodes; //0x0290
	char pad_0294[4]; //0x0294
	class CNetBlacklistNode* m_start; //0x0298
	char pad_02A0[24]; //0x02A0
}; //Size: 0x02B8
static_assert(sizeof(CNetBlacklist) == 0x2B8);
class RemotePlayerData {
public:
	class rage::netGamePlayerData m_data[32]; //0x0000
	uint32_t m_count; //0x0600
	char pad_0604[4]; //0x0604
}; //Size: 0x0608
static_assert(sizeof(RemotePlayerData) == 0x608);
class InvitedGamer {
public:
	class rage::rlGamerHandle m_handle;
	char pad_0010[12]; //0x0010
	uint32_t m_flags; //0x001C
}; //Size: 0x0020
static_assert(sizeof(InvitedGamer) == 0x20);
class InvitedGamers {
public:
	class InvitedGamer m_invited_gamers[100]; //0x0000
	uint32_t m_num_invited_gamers; //0x0C80
	char pad_0C84[4]; //0x0C84
}; //Size: 0x0C88
static_assert(sizeof(InvitedGamers) == 0xC88);
class Network {
public:
	rage::rlSessionInfo m_steam_unk_session; //0x0000
	rage::Obf32 m_num_dinput8_instances; //0x0070
	rage::Obf32 m_last_time_dinput8_checked; //0x0080
	class rage::snSession m_game_session; //0x00F0
	class rage::snSession m_transition_session; //0x5578
	char pad_AA00[16]; //0xAA00
	class rage::snSession* m_game_session_ptr_2; //0xAA10
	class rage::snSession* m_transition_session_ptr_2; //0xAA18
	char pad_AA20[16]; //0xAA20
	class rage::snSession* m_game_session_ptr; //0xAA30
	class rage::snSession* m_transition_session_ptr; //0xAA38
	char pad_AA40[24]; //0xAA40
	class NetworkGameConfig m_network_game_config; //0xAA58
	class NetworkGameConfig m_network_transition_config; //0xABA0
	bool m_session_attributes_dirty; //0xACE8
	char pad_ACE9[19]; //0xACE9
	uint32_t m_session_visibility_flags; //0xACFC
	uint32_t m_transition_visibility_flags; //0xAD00
	char pad_AD04[68]; //0xAD04
	class MetricSessionMigrated m_metric_session_migrated; //0xAD48
	bool m_migrated_metric_enabled; //0xB090
	char pad_B091[3]; //0xB091
	uint32_t m_game_session_state; //0xB094
	class NetworkGameFilter m_network_game_filter; //0xB098
	char pad_B3DC[33]; //0xB3DC
	bool m_was_invited; //0xB3FD
	char pad_B3FE[10]; //0xB3FE
	class rage::rlSessionInfo m_unk_session_info; //0xB408
	char pad_B4D8[635]; //0xB4D8
	bool m_need_host_change; //0xB753
	char pad_B754[74316]; //0xB754
	class rage::rlSessionDetail m_joining_session_detail; //0x1D9A0
	class SessionInfoBackup m_last_joined_session; //0x1DE18
	char pad_1DEF4[40]; //0x1DEF4
	uint32_t m_current_matchmaking_group; //0x1DF1C
	uint32_t m_matchmaking_group_max_players[5]; //0x1DF20
	uint32_t m_num_active_matchmaking_groups; //0x1DF34
	char pad_1DF38[8]; //0x1DF38
	uint8_t m_matchmaking_property_id; //0x1DF40
	uint8_t m_matchmaking_mental_state; //0x1DF41
	char pad_1DF42[366]; //0x1DF42
	class rage::rlMatchmakingFindResult m_game_session_matchmaking[3]; //0x1E0B0
	char pad_2ABC0[40]; //0x2ABC0
	class MatchmakingSessionResult m_game_matchmaking_session_results[10]; //0x2ABE8
	char pad_2D988[308]; //0x2D988
	uint32_t m_num_bosses; //0x2DABC
	bool m_num_bosses_set; //0x2DAC0
	char pad_2DAC1[7]; //0x2DAC1
	class rage::rlGamerHandle m_transition_creator_handle; //0x2DAC8
	char pad_2DAD8[12]; //0x2DAD8
	bool m_is_waiting_async; //0x2DAE4
	bool m_is_preferred_activity; //0x2DAE5
	char pad_2DAE6[2]; //0x2DAE6
	uint32_t m_in_progress_finish_time; //0x2DAE8
	char pad_2DAEC[4]; //0x2DAEC
	bool m_local_player_info_dirty; //0x2DAF0
	char pad_2DAF1[495]; //0x2DAF1
	class rage::rlGamerHandle m_inviter_handle; //0x2DCE0
	class CNetComplaintMgr m_game_complaint_mgr; //0x2DCF0
	class CNetComplaintMgr m_transition_complaint_mgr; //0x2EB88
	char pad_2FA20[32]; //0x2FA20
	class JoiningPlayerNameMap m_unused_joining_player_name_map; //0x2FA40
	char pad_31048[8]; //0x31048
	class CNetBlacklist m_blacklist; //0x31050
	char pad_31308[8]; //0x31308
	class InvitedGamers m_game_invited_gamers; //0x31310
	char pad_31F98[5888]; //0x31F98
	class SessionInfoBackup m_last_joined_transition; //0x33698
	uint32_t m_activity_max_players; //0x33774
	uint32_t m_activity_max_spectators; //0x33778
	char pad_3377C[48]; //0x3377C
	bool m_do_not_launch_from_join_as_migrated_host; //0x337AC
	char pad_337AD[7]; //0x337AD
	bool m_is_activity_session; //0x337B4
	char pad_337B5[35]; //0x337B5
	class RemotePlayerData m_remote_player_data; //0x337D8
	char pad_33DE0[8]; //0x33DE0
	class rage::netGamePlayerData m_local_net_game_player_data; //0x33DE8
	char pad_33E18[608]; //0x33E18
	class rage::rlMatchmakingFindResult m_transition_matchmaking[4]; //0x34078
	class NetworkGameFilter m_transition_filters[4]; //0x44F38
	char pad_45C48[20]; //0x45C48
	uint32_t m_transition_quickmatch_group_handle_count; //0x45C5C
	class rage::rlGamerHandle m_transition_quickmatch_group_handles[32]; //0x45C60
	bool m_retain_activity_group; //0x45E60
	char pad_45E61[7]; //0x45E61
	class rage::rlSessionInfo m_transition_to_activity_session_info; //0x45E68
	char pad_45F38[48]; //0x45F38
	class MatchmakingSessionResult m_transition_matchmaking_session_results[10]; //0x45F68
	char pad_48D08[8]; //0x48D08
	class InvitedGamers m_transition_invited_gamers; //0x48D10
	char pad_49998[16]; //0x49998
	class rage::rlGamerHandle m_transition_to_game_handle; //0x499A8
	class rage::rlSessionInfo m_transition_to_game_session_info; //0x499B8
	char pad_49A88[4]; //0x49A88
	uint32_t m_transition_to_game_session_participant_count; //0x49A8C
	class rage::rlGamerHandle m_transition_to_game_session_participants[32]; //0x49A90
	char pad_49C90[80]; //0x49C90
	class rage::rlGamerHandle m_follower_handles[32]; //0x49CE0
	uint32_t m_follower_count; //0x49EE0
	char pad_49EE4[628]; //0x49EE4
}; //Size: 0x38650
static_assert(sizeof(Network) == 0x4A158);
#pragma pack(pop)

#pragma pack(push, 1)
class ScInfo {
public:
	char m_ticket[208]; //0x0000
	char pad_00D0[304]; //0x00D0
	char m_session_ticket[88]; //0x0200
	char pad_0258[40]; //0x0258
	uint32_t m_nonce; //0x0280
	char pad_0284[4]; //0x0284
	uint32_t m_account_id; //0x0288
	char pad_028C[16]; //0x028C
	char m_profile_pic[128]; //0x029C
	char pad_031C[32]; //0x031C
	char m_country_code[4]; //0x033C
	char pad_0340[31]; //0x0340
	char m_email_address[96]; //0x035F
	char pad_03BF[6]; //0x03BF
	char m_language_subtag[8]; //0x03C5
	char pad_03CD[2]; //0x03CD
	char m_sc_name[20]; //0x03CF
	char pad_03E3[533]; //0x03E3
	char m_session_key[16]; //0x05F8
	char pad_0608[2296]; //0x0608
}; //Size: 0x0F00
static_assert(sizeof(ScInfo) == 0xF00);
#pragma pack(pop)
#pragma pack(push, 4)
class PresenceData {
public:
	uintptr_t m_base_addr; //0x0000
	char pad_0008[4]; //0x0008
	int m_offset; //0x000C
	int m_max_str_len; //0x0010
};
#pragma pack(pop)
#pragma pack(push, 1)
class CMsgTransitionToGameNotify {
public:
	CMsgTransitionToGameNotify(rage::rlGamerHandle handle, rage::rlSessionInfo sessionInfo, uint8_t type) : m_gamer_handle(handle), m_session_info(sessionInfo), m_type(type), m_is_session_unk(false) {}
public:
	bool serialize(rage::datBitBuffer& buffer) {
		m_gamer_handle.serialize(buffer);
		m_session_info.serialize(buffer);
		buffer.Write<uint8_t>(m_type, 8);
		buffer.WriteBool(&m_is_session_unk);
	}
	bool deserialize(rage::datBitBuffer& buffer) {
		m_gamer_handle.deserialize(buffer);
		m_session_info.deserialize(buffer);
		m_type = buffer.Read<uint8_t>(8);
		buffer.ReadBool(&m_is_session_unk);
	}
public:
	rage::rlGamerHandle m_gamer_handle; //0x0000
	rage::rlSessionInfo m_session_info; //0x0010
	uint8_t m_type; //0x00E0
	bool m_is_session_unk; //0x00E1
}; //Size: 0x00E2
static_assert(sizeof(CMsgTransitionToGameNotify) == 0xE2);
#pragma pack(pop)
namespace rage {
	class netTime {
	public:
		virtual ~netTime() = default;

		netConnectionManager* m_net_connection_mgr; //0x0008
		uint32_t m_host_peer_id; //0x0010
		uint32_t m_time_token; //0x0014
		uint32_t m_time_offset; //0x0018
		char pad_001C[72]; //0x001C
		uint32_t m_failed_sync_counter; //0x0064
		uint32_t m_last_sync_id_sent; //0x0068
		uint32_t m_last_sync_id_received; //0x006C
		uint32_t m_role_flags; //0x0070
		uint32_t m_connection_identifier; //0x0074
		uint32_t m_time; //0x0078
		uint32_t m_calculation_flags; //0x007C
	}; //Size: 0x0080
	static_assert(sizeof(netTime) == 0x80);
	struct netTimeSyncMsg {
		int32_t action;
		int32_t counter;
		int32_t token;
		int32_t timestamp;
		int32_t increment;
	};
	static_assert(sizeof(netTimeSyncMsg) == 0x14);
}