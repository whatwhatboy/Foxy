#pragma once
#include "AtArray.hpp"
#include "Joaat.hpp"
#include "SCRThread.hpp"
#include "Base.hpp"
#include "BitBuffer.hpp"

namespace rage {
	class datBitBuffer;
	class scriptId;
	class scriptHandlerObject;
	class scriptResource;
	class netLoggingInterface;
	class netPlayer;
	class scriptHandlerMgr {
	public:
		virtual ~scriptHandlerMgr();
		virtual bool initialize();
		virtual void _0x10();
		virtual void shutdown();
		virtual void _0x20();
		virtual void _0x28();
		virtual void _0x30();
		virtual void _0x38(scrThread*);
		virtual scriptHandler* create_script_handler();
		virtual scriptHandler* get_script_handler(scriptId* id);
		virtual void attach_thread(scrThread* thread);
		virtual void detach_thread(scrThread* thread);
		virtual void on_player_join(netPlayer* player);
		virtual void on_player_left(netPlayer* player);
		virtual int32_t _0x70();
		virtual void* _0x78();
	public:
		char pad_0008[40]; //0x0008
		bool m_initialized; //0x0030
		bool m_initialized_2; //0x0031
		char pad_0032[14]; //0x0032
		netLoggingInterface* m_logger; //0x0040
	}; //Size: 0x0048
	//Resource
	class scriptResource {
	public:
		virtual ~scriptResource();
	}; //Size: 0x0008
	static_assert(sizeof(scriptResource) == 0x8);
	//Resource Entry
	class scriptResourceEntry {
	public:
		scriptResource* m_data; //0x0000
		uint32_t m_unk; //0x0004
		char pad_000C[12]; //0x000C
		scriptResourceEntry* m_next; //0x0018
	}; //Size: 0x0020
	#pragma pack(push, 1)
	class scriptHandlerNetComponent {
	public:
		virtual ~scriptHandlerNetComponent();
		virtual bool _0x08(void*) = 0;
		virtual void _0x10(CNetGamePlayer*) = 0; //creates a scriptId?
		virtual void* player_left(CNetGamePlayer* player) = 0;
		virtual void* send_host_migration_event(CNetGamePlayer* player) = 0;
		virtual void* player_joined(void**, void* msg_ctx) = 0;
		virtual void* player_joined_ack(void**, void* msg_ctx) = 0;
		virtual bool _0x38(void*, void*) = 0; //join_script?
		virtual void* _0x40(void*, void*) = 0;
		virtual void* _0x48(void*, void*, void*) = 0;
		virtual void* _0x50(void*, void*) = 0;
		virtual void* _0x58(void*, void*) = 0;
		virtual void* _0x60(void*, void*) = 0;
		virtual void* _0x68(void*, void*) = 0;
		virtual void _0x70(void*, void*) = 0;
		virtual void _0x78(void*, void*) = 0;
		virtual short _0x80(void*, void*) = 0;
		virtual void* _0x88(void*, void*) = 0;
		virtual void* _0x90(void*, void*) = 0; //HOST_MIGRATION_FAILED
		virtual bool _0x98(void*, void*) = 0;
		virtual void* _0xA0(void*, void*) = 0;
		virtual void* _0xA8(void*, void*) = 0;
		virtual short _0xB0(void*, void*) = 0;
		virtual bool register_host_broadcast_data(int* data, int size, char* debugString) = 0;
		virtual bool register_player_broadcast_data(int data, int size, bool sync) = 0;
		virtual bool _0xC8() = 0; // something to do to joining session
		virtual bool _0xD0() = 0;
		virtual bool add_player_to_script(CNetGamePlayer* player, short* outParticipantID, short* outSlot, int* outFailReason) = 0;
		virtual bool add_player_to_script_internal(CNetGamePlayer* player, short participantID, short slot) = 0; // player aka participant
		virtual bool remove_player_from_script(CNetGamePlayer* player) = 0;
		virtual void* player_left_impl(CNetGamePlayer*, bool) = 0;
		virtual bool do_host_migration(CNetGamePlayer* player, short host_token, bool unk) = 0; // aka _0xF8
		virtual void* leave_from_script() = 0; // calls above function with player = nullptr
		virtual bool _0x108() = 0;
		virtual void* _0x110() = 0;
		virtual bool _0x118() = 0; // related to above function
	public:
		scriptHandler* m_script_handler; //0x0008
	}; //Size: 0x0010
	static_assert(sizeof(scriptHandlerNetComponent) == 0x10);
	#pragma pack(pop)
	class scriptHandler {
	public:
		class atDScriptObjectNode : public atDNode<scriptHandlerObject*> {};
	public:
		virtual ~scriptHandler();
		virtual bool _0x08();
		virtual void _0x10();
		virtual void cleanup_objects();
		virtual scriptId* _0x20();
		virtual scriptId* get_id();
		virtual bool is_networked();
		virtual void init_net_component();
		virtual void reset_net_component();
		virtual bool destroy();
		virtual void add_object(scriptHandlerObject*, bool is_network, bool is_network_and_scriptcheck);
		virtual void _0x58(void*);
		virtual void register_resource(scriptResource*, void*);
		virtual void _0x68();
		virtual void _0x70();
		virtual void _0x78();
		virtual void _0x80();
		virtual void _0x88();
		virtual void _0x90();
		virtual void _0x98();
	public:
		void* unk_0008; //0x0008
		void* unk_0010; //0x0010
		scrThread* m_script_thread; //0x0018
		atDList<atDScriptObjectNode> m_objects; //0x0020
		scriptResource* m_resource_list_head; //0x0030
		scriptResource* m_resource_list_tail; //0x0038
		void* unk_0040; //0x0040
		scriptHandlerNetComponent* m_net_component; //0x0048
		uint32_t unk_0050; //0x0050
		uint32_t unk_0054; //0x0054
		uint32_t unk_0058; //0x0058
		uint32_t unk_005C; //0x005C
	}; //Size: 0x0060
	static_assert(sizeof(scriptHandler) == 0x60);
	//Id Base
	class scriptIdBase {
	public:
		virtual ~scriptIdBase();
		virtual void assume_thread_identity(scrThread* thread);
		virtual bool is_valid();
		virtual joaat_t* get_hash(joaat_t* out);
		virtual joaat_t* get_hash2(joaat_t* out);
		virtual const char* get_name();
		virtual void deserialize(datBitBuffer* buffer);
		virtual void serialize(datBitBuffer* buffer);
		virtual uint32_t _0x40();
		virtual uint32_t _0x48();
		virtual void log_information(void* logger);
		virtual void copy_data(scriptIdBase* other);
		virtual bool operator==(scriptIdBase*);
		virtual bool _0x68(void*);
	}; //Size: 0x0008
	static_assert(sizeof(scriptIdBase) == 0x8);
	#pragma pack(pop)
	#pragma pack(push, 1)
	class scriptId : public scriptIdBase {
	public:
		uint32_t m_hash; //0x0008
		char m_name[32]; //0x000C
	}; //Size: 0x002C
	static_assert(sizeof(scriptId) == 0x2C);
	#pragma pack(pop)
}
#pragma pack(push, 1)
class CGameScriptId : public rage::scriptId {
public:
	char pad_002C[4]; //0x002C
	int32_t m_timestamp; //0x0030
	int32_t m_position_hash; //0x0034
	int32_t m_instance_id; //0x0038
	int32_t m_unk; //0x003C
}; //Size: 0x0040
static_assert(sizeof(CGameScriptId) == 0x40);
#pragma pack(pop)
#pragma pack(push, 1)
class CRemoteScriptInfo : public CGameScriptId {
public:
	uint32_t m_participants; //0x0040
	char pad_0044[4]; //0x0044
	class CNetGamePlayer* m_host; //0x0048
	uint32_t m_timestamp_2; //0x0050
	uint16_t m_host_token; //0x0054
	uint8_t m_reserved_peds; //0x0056
	uint8_t m_reserved_vehicles; //0x0057
	uint8_t m_reserved_objects; //0x0058
	uint8_t m_reserved_unk1; //0x0059
	uint8_t m_reserved_unk2; //0x005A
	uint8_t m_reserved_unk3; //0x005B
	uint8_t m_reserved_unk4; //0x005C
	char pad_005D[3]; //0x005D
}; //Size: 0x0060
static_assert(sizeof(CRemoteScriptInfo) == 0x60);
#pragma pack(pop)

class CGameScriptHandler : public rage::scriptHandler {
public:
	CGameScriptId m_script_id; //0x0060
};

class CGameScriptHandlerNetwork : public CGameScriptHandler {
public:
	uint8_t unk_00A0; //0x00A0
	uint8_t unk_00A1; //0x00A1
	uint8_t unk_00A2; //0x00A2
	uint8_t unk_00A3; //0x00A3
	uint8_t m_num_players; //0x00A4
	uint8_t unk_00A5; //0x00A5
	uint8_t unk_00A6; //0x00A6
	uint8_t unk_00A7; //0x00A7
	uint8_t unk_00A8; //0x00A8
	uint8_t unk_00A9; //0x00A9
	uint8_t unk_00AA; //0x00AA
	uint8_t unk_00AB; //0x00AB
	uint8_t unk_00AC; //0x00AC
	uint8_t unk_00AD; //0x00AD
	uint8_t unk_00AE; //0x00AE
	uint8_t unk_00AF; //0x00AF
};
class CGameScriptHandlerMgr : public rage::scriptHandlerMgr {};
class CScriptParticipant {
public:
	char pad_0000[16]; //0x0000
	class CNetGamePlayer* m_net_game_player; //0x0010
	char pad_0018[2]; //0x0018
	int16_t m_participant_index; //0x001A
	char pad_001C[12]; //0x001C
}; //Size: 0x0028
static_assert(sizeof(CScriptParticipant) == 0x28);
class CGameScriptHandlerNetComponent : public rage::scriptHandlerNetComponent {
public:
	char pad_0010[32]; //0x0010
	class CScriptParticipant* m_host; //0x0030
	int16_t m_local_participant_index; //0x0038
	char pad_003A[6]; //0x003A
	uint32_t m_participant_bitset; //0x0040
	char pad_0044[36]; //0x0044
	class CScriptParticipant* m_participants[32]; //0x0068
	char pad_0168[12]; //0x0168
	int16_t m_num_participants; //0x0174
	char pad_0176[28]; //0x0176
	uint8_t m_host_migration_flags; //0x0192
	char pad_0193[29]; //0x0193
	int get_participant_index(CNetGamePlayer* player) {
		if (player == (*base::pointers::g_networkPlayerMgr)->m_local_net_player)
			return m_local_participant_index;
		if (m_num_participants <= 1)
			return -1;
		for (decltype(m_num_participants) i{}; i != m_num_participants - 1; ++i) {
			if (m_participants[i] && m_participants[i]->m_net_game_player == player)
				return m_participants[i]->m_participant_index;
		}
		return -1;
	}
	bool is_player_a_participant(CNetGamePlayer* player) {
		return m_participant_bitset & (1 << player->m_player_id);
	}
	bool is_local_player_host() {
		if (!m_host)
			return true;

		return m_host->m_participant_index == m_local_participant_index;
	}
	bool is_player_host(CNetGamePlayer* player) {
		return m_host->m_participant_index == get_participant_index(player);
	}
	CNetGamePlayer* get_host() {
		if (!m_host)
			return nullptr;

		return m_host->m_net_game_player;
	}
	void block_host_migration(bool toggle) {
		if (toggle)
			m_host_migration_flags |= (1 << 7);
		else
			m_host_migration_flags &= ~(1 << 7);
	}
	bool force_host() {
		if (auto& cNetworkPlayerMgr = *base::pointers::g_networkPlayerMgr; cNetworkPlayerMgr) {
			for (int32_t i{}; !is_local_player_host(); ++i) {
				if (i > 200)
					return false;

				send_host_migration_event(cNetworkPlayerMgr->m_local_net_player);
				send_host_migration_event(cNetworkPlayerMgr->m_local_net_player);
			}
			return is_local_player_host();
		}
		return false;
	}
	bool give_host(CNetGamePlayer* player) {
		if (!force_host())
			return false;
		send_host_migration_event(player);
		do_host_migration(player, NULL, true);
		return is_player_host(player);
	}
}; //Size: 0x01B0
static_assert(sizeof(CGameScriptHandlerNetComponent) == 0x1B0);