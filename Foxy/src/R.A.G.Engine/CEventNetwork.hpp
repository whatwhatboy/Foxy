#pragma once
#include <cstdint>
#include "Natives/NativeDefines.hpp"
#include "RTTI.hpp"

namespace rage {
	class sEntityDamagedData {
	public:
		alignas(8) Entity m_victim_index; //0x0000
		alignas(8) Entity m_damager_index; //0x0008
		alignas(8) float m_damage; //0x0010
		alignas(8) float m_endurance_damage; //0x0018
		alignas(8) bool m_victim_incapacitated; //0x0020
		alignas(8) bool m_victim_destroyed; //0x0028
		alignas(8) int m_weapon_used; //0x0030
		alignas(8) float m_victim_speed; //0x0038
		alignas(8) float m_damager_speed; //0x0040
		alignas(8) bool m_is_responsible_for_collision; //0x0048
		alignas(8) bool m_is_headshot; //0x0050
		alignas(8) bool m_is_with_melee_weapon; //0x0058
		alignas(8) int m_hit_material; //0x0060
	}; //Size: 0x0068
	static_assert(sizeof(sEntityDamagedData) == 0x68);
	class sEventPlayerSpawn {
	public:
		char pad_0000[24]; //0x0000
		char m_name[20]; //0x0018
		char pad_002C[12]; //0x002C
	}; //Size: 0x0038
	static_assert(sizeof(sEventPlayerSpawn) == 0x38);
	class sEventPlayerCollectedAmbientPickup {
	public:
		alignas(8) joaat_t m_pickup_type; //0x0000
		alignas(8) int m_pickup_amount; //0x0008
		alignas(8) Player m_player_index; //0x0010
		alignas(8) joaat_t m_pickup_model; //0x0018
		alignas(8) bool m_player_gift; //0x0020
		alignas(8) bool m_dropped_by_ped; //0x0028
		alignas(8) int m_pickup_collected; //0x0030
		alignas(8) Pickup m_pickup_index; //0x0038
	}; //Size: 0x0040
	static_assert(sizeof(sEventPlayerCollectedAmbientPickup) == 0x40);
	enum class eEventNetworkType : int64_t {
		CEventNetworkPlayerJoinScript = 153,
		CEventNetworkPlayerLeftScript = 154,
		CEventNetworkStorePlayerLeft = 155,
		CEventNetworkStartSession = 156,
		CEventNetworkEndSession = 157,
		CEventNetworkStartMatch = 158,
		CEventNetworkRemovedFromSessionDueToStall = 160,
		CEventNetworkRemovedFromSessionDueToComplaints = 161,
		CEventNetworkConnectionTimeout = 162,
		CEventNetworkPedDroppedWeapon = 163,
		CEventNetworkPlayerSpawn = 164,
		CEventNetworkPlayerCollectedPickup = 165,
		CEventNetworkPlayerCollectedAmbientPickup = 166,
		CEventNetworkPlayerCollectedPortablePickup = 167,
		CEventNetworkPlayerDroppedPortablePickup = 168,
		CEventNetworkInviteAccepted = 170,
		CEventNetworkInviteConfirmed = 171,
		CEventNetworkInviteRejected = 172,
		CEventNetworkSummo = 173,
		CEventNetworkScriptEvent = 174,
		CEventNetworkSignInStateChanged = 176,
		CEventNetworkSignInChangeActioned = 177,
		CEventNetworkRosChanged = 178,
		CEventNetworkBail = 179,
		CEventNetworkHostMigration = 180,
		CEventNetworkFindSession = 181,
		CEventNetworkHostSession = 182,
		CEventNetworkJoinSession = 183,
		CEventNetworkJoinSessionResponse = 184,
		CEventNetworkCheatTriggered = 185,
		CEventNetworkEntityDamage = 186,
		CEventNetworkPlayerArrest = 187,
		CEventNetworkTimedExplosion = 188,
		CEventNetworkPrimaryClanChanged = 189,
		CEventNetworkClanJoined = 190,
		CEventNetworkClanLeft = 191,
		CEventNetworkClanInviteReceived = 192,
		CEventNetworkVoiceSessionStarted = 193,
		CEventNetworkVoiceSessionEnded = 194,
		CEventNetworkVoiceConnectionRequested = 195,
		CEventNetworkVoiceConnectionResponse = 196,
		CEventNetworkVoiceConnectionTerminated = 197,
		CEventNetworkTextMessageReceived = 198,
		CEventNetworkCloudFileResponse = 199,
		CEventNetworkPickupRespawned = 200,
		CEventNetworkPresence_StatUpdate = 201,
		CEventNetworkPedLeftBehind = 202,
		CEventNetwork_InboxMsgReceived = 203,
		CEventNetworkAttemptHostMigration = 204,
		CEventNetworkIncrementStat = 205,
		CEventNetworkSessionEvent = 206,
		CEventNetworkTransitionStarted = 207,
		CEventNetworkTransitionEvent = 208,
		CEventNetworkTransitionMemberJoined = 209,
		CEventNetworkTransitionMemberLeft = 210,
		CEventNetworkTransitionParameterChanged = 211,
		CEventNetworkClanKicked = 212,
		CEventNetworkTransitionStringChanged = 213,
		CEventNetworkTransitionGamerInstruction = 214,
		CEventNetworkPresenceInvite = 215,
		CEventNetworkPresenceInviteRemoved = 216,
		CEventNetworkPresenceInviteReply = 217,
		CEventNetworkCashTransactionLog = 218,
		CEventNetworkClanRankChanged = 219,
		CEventNetworkVehicleUndrivable = 220,
		CEventNetworkPresenceTriggerEvent = 221,
		CEventNetworkEmailReceivedEvent = 222,
		CEventNetworkFollowInviteReceived = 223,
		CEventNetworkSpectateLocal = 225,
		CEventNetworkCloudEvent = 226,
		CEventNetworkShopTransaction = 227,
		CEventNetworkOnlinePermissionsUpdated = 230,
		CEventNetworkSystemServiceEvent = 231,
		CEventNetworkRequestDelay = 232,
		CEventNetworkSocialClubAccountLinked = 233,
		CEventNetworkScAdminPlayerUpdated = 234,
		CEventNetworkScAdminReceivedCash = 235,
		CEventNetworkClanInviteRequestReceived = 236,
		CEventNetworkMarketingEmailReceivedEvent = 237,
		CEventNetworkStuntPerformed = 238,
		CEventNetworkFiredDummyProjectile = 239,
		CEventNetworkPlayerEnteredVehicle = 240,
		CEventNetworkPlayerActivatedSpecialAbility = 241,
		CEventNetworkPlayerDeactivatedSpecialAbility = 242,
		CEventNetworkPlayerSpecialAbilityFailedActivation = 243,
		CEventNetworkFiredVehicleProjectile = 244,
	};
	class CEventNetwork : public atRTTI<CEventNetwork> {
	public:
		virtual ~CEventNetwork() = default; //0x0000 (0)
		virtual void unk_0008() = 0; //0x0008 (1)
		virtual void unk_0010() = 0; //0x0010 (2)
		virtual eEventNetworkType get_type() = 0; //0x0018 (3)
		virtual void unk_0020() = 0; //0x0020 (4)
		virtual void unk_0028() = 0; //0x0028 (5)
		virtual bool get_extra_information(void* info_array, int check) = 0; //0x0030 (6)
		virtual void unk_0038() = 0; //0x0038 (7)
	}; //Size: 0x0008
}