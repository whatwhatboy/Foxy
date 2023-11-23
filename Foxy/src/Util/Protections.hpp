#pragma once
#include "Hooking/Hooking.hpp"
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"

namespace base {
	inline std::vector<std::string> m_toggleTypes = {
		"Notify and Block",
		"Notify",
		"Block"
	};
	enum eToggleTypes {
		tog_notifyAndBlock = 1,
		tog_notify,
		tog_block
	};
	struct toggleHandler { bool toggle = false; int toggleType = tog_notifyAndBlock; };
	struct eventHandler { std::string name; std::int32_t hash; toggleHandler& toggle; };
	struct modelHandler { std::string name; std::uint32_t hash; toggleHandler& toggle; };
	namespace protections {
		inline bool g_logScriptEvents{};
		inline bool g_killAllSyncs{};
		//Script Events
		inline toggleHandler g_blockCEOKick{};
		inline toggleHandler g_blockCEOBan{};
		inline toggleHandler g_blockCEOMoney{};
		inline toggleHandler g_blockSetBounty{};
		inline toggleHandler g_blockClearWanted{};
		inline toggleHandler g_blockFakeDeposit{};
		inline toggleHandler g_blockForceMission{};
		inline toggleHandler g_blockForceMission2{};
		inline toggleHandler g_blockGiveCollectable{};
		inline toggleHandler g_blockGTABanner{};
		inline toggleHandler g_blockNetworkBail{};
		inline toggleHandler g_blockPersonalVehDestory{};
		inline toggleHandler g_blockRemoteOffRadar{};
		inline toggleHandler g_blockRotateCamera{};
		inline toggleHandler g_blockSendToCutscene{};
		inline toggleHandler g_blockSendToIsland{};
		inline toggleHandler g_blockSendToLocation{};
		inline toggleHandler g_blockSoundSpam{};
		inline toggleHandler g_blockSpectate{};
		inline toggleHandler g_blockTeleport{};
		inline toggleHandler g_blockTeleportToWarehouse{};
		inline toggleHandler g_blockMCTeleport{};
		inline toggleHandler g_blockStartActivity{};
		inline toggleHandler g_blockMarkPlayerAsBeast{};
		inline toggleHandler g_blockKickFromInterior{};
		inline toggleHandler g_blockInterriorControl{};
		inline toggleHandler g_blockTransactionError{};
		inline toggleHandler g_blockVehKick{};
		inline toggleHandler g_blockStoryKick{};
		inline toggleHandler g_blockStoryKick2{};
		inline toggleHandler g_blockStoryKick3{};
		inline toggleHandler g_blockCrash1{};
		inline toggleHandler g_blockCrash2{};
		inline toggleHandler g_blockTSECommand{};
		inline toggleHandler g_blockTSECommandRotateCamera{};
		inline std::vector<eventHandler> m_scriptEvents = {
			{ "CEO Kick", eScriptEvents::CEO_KICK, g_blockCEOKick },
			{ "CEO Ban", eScriptEvents::CEO_BAN, g_blockCEOBan },
			{ "CEO Money", eScriptEvents::CEO_MONEY, g_blockCEOMoney },
			{ "Set Bounty", eScriptEvents::SET_BOUNTY, g_blockSetBounty },
			{ "Clear Wanted", eScriptEvents::CLEAR_WANTED, g_blockClearWanted },
			{ "Fake Deposit", eScriptEvents::FAKE_DEOPOSIT, g_blockFakeDeposit },
			{ "Force Mission", eScriptEvents::FORCE_MISSION, g_blockForceMission },
			{ "Force Mission 2", eScriptEvents::FORCE_MISSION_2, g_blockForceMission },
			{ "Give Collectable", eScriptEvents::GIVE_COLLECTIBLE, g_blockGiveCollectable },
			{ "GTA Banner", eScriptEvents::GTA_BANNER, g_blockGTABanner },
			{ "Network Bail", eScriptEvents::NETWORK_BAIL, g_blockNetworkBail },
			{ "Personal Veh Destory", eScriptEvents::PERSONAL_VEH_DESTORY, g_blockPersonalVehDestory },
			{ "Remote Off Radar", eScriptEvents::REMOTE_OFFRADAR, g_blockRemoteOffRadar },
			{ "Rotate Camera", eScriptEvents::ROTATE_CAMERA, g_blockRotateCamera },
			{ "Send To (Cutscene)", eScriptEvents::SEND_TO_CUSTSCENE, g_blockSendToCutscene },
			{ "Send To (Island)", eScriptEvents::SEND_TO_ISLAND, g_blockSendToIsland },
			{ "Send To (Location)", eScriptEvents::SEND_TO_LOCATION, g_blockSendToLocation },
			{ "Sound Spam", eScriptEvents::SOUND_SPAM, g_blockSoundSpam },
			{ "Spectate", eScriptEvents::SPECTATE, g_blockSpectate },
			{ "Teleport", eScriptEvents::TELEPORT, g_blockTeleport },
			{ "Teleport To Warehouse", eScriptEvents::TELEPORT_TO_WAREHOUSE, g_blockTeleportToWarehouse },
			{ "MC Teleport", eScriptEvents::MC_TELEPORT, g_blockMCTeleport },
			{ "Start Activity", eScriptEvents::START_ACTIVITY, g_blockStartActivity },
			{ "Mark Player As Beast", eScriptEvents::MARK_PLAYER_AS_BEAST, g_blockMarkPlayerAsBeast },
			{ "Kick From Interior", eScriptEvents::KICK_FROM_INTERIOR, g_blockKickFromInterior },
			{ "Interior Control", eScriptEvents::INTERIOR_CONTROL, g_blockInterriorControl },
			{ "Transaction Error", eScriptEvents::TRANSACTION_ERROR, g_blockTransactionError },
			{ "Veh Kick", eScriptEvents::VEH_KICK, g_blockVehKick },
			{ "Kick To Story", eScriptEvents::KICK, g_blockStoryKick },
			{ "Kick To Story (2)", eScriptEvents::KICK_2, g_blockStoryKick2 },
			{ "Kick To Story (3)", eScriptEvents::KICK_3, g_blockStoryKick3 },
			{ "Crash", eScriptEvents::CRASH, g_blockCrash1 },
			{ "Crash (2)", eScriptEvents::CRASH_2, g_blockCrash2 },
			{ "TSE Command", eScriptEvents::TSE_COMMAND, g_blockTSECommand },
			{ "TSE Command (Rotate Camera)", eScriptEvents::TSE_COMMAND_ROTATE_CAMERA, g_blockTSECommandRotateCamera }
		};
		//Report Events
		inline toggleHandler g_blockReports{};
		inline std::vector<modelHandler> m_reportHashes = {
			{ "Bad Crew Status", "MPPLY_BAD_CREW_STATUS"_joaat, g_blockReports },
			{ "Bad Crew Motto", "MPPLY_BAD_CREW_MOTTO"_joaat, g_blockReports },
			{ "Bad Crew Name", "MPPLY_BAD_CREW_NAME"_joaat, g_blockReports },
			{ "Bad Crew Emblem", "MPPLY_BAD_CREW_EMBLEM"_joaat, g_blockReports },
			{ "Annoying Me In VC", "MPPLY_VC_ANNOYINGME"_joaat, g_blockReports },
			{ "VC Hate", "MPPLY_VC_HATE"_joaat, g_blockReports },
			{ "Exploits", "MPPLY_EXPLOITS"_joaat, g_blockReports },
			{ "Offsensive Playermade Title", "MPPLY_PLAYERMADE_TITLE"_joaat, g_blockReports },
			{ "Offsensive Playermade Description", "MPPLY_PLAYERMADE_DESC"_joaat, g_blockReports }
		};
		//Objects
		inline toggleHandler g_blockSlodCrash{};
		inline std::vector<modelHandler> m_crashModels = {
			{ "Human (Slod)", "slod_human"_joaat, g_blockSlodCrash },
			{ "Large Quadped (Slod)", "slod_large_quadped"_joaat, g_blockSlodCrash },
			{ "Small Quadped (Slod)", "slod_small_quadped"_joaat, g_blockSlodCrash }
		};
		//Kicks
		inline toggleHandler g_blockRemovePlayerKick{};
		inline toggleHandler g_blockRaiseComplaintKick{};
		//Crashes
		inline toggleHandler g_blockHostCrash{};
		inline toggleHandler g_blockInvalidAttachmentCrash{};
		inline toggleHandler g_blockNextGenVehicleCrash{};
		inline toggleHandler g_blockNextGenPedCrash{};
		inline toggleHandler g_blockInvalidPlayerModelCrash{};
		inline toggleHandler g_blockInvalidDoorModelCrash{};
		inline toggleHandler g_blockInvalidPedPropModelCrash{};
		inline toggleHandler g_blockSyncMismatchCrash{};
		inline toggleHandler g_blockOutOfRangeSyncCrash{};
		inline toggleHandler g_blockVehicleTaskCrash{};
		inline toggleHandler g_blockGiveControlCrash{};
		//Network events
		inline toggleHandler g_blockExplosions{};
		inline toggleHandler g_blockFreeze{};
		inline toggleHandler g_blockRequestControl{};
		inline toggleHandler g_blockGiveWeapon{};
		inline toggleHandler g_blockRemoveWeapon{};
		inline toggleHandler g_blockRemoveAllWeapons{};
		inline toggleHandler g_blockVoteKicks{};
		inline toggleHandler g_blockAlterWantedLvl{};
		inline toggleHandler g_blockClearArea{};
		//Misc stuff
		inline toggleHandler g_playerBlacklist{};
		inline bool g_blockPlayersFromRIDJoiningInPrivateSessions{};
		class blacklist {
		public:
			blacklist(CNetGamePlayer* player, const char* reason) : m_name(player->GetName()), m_blacklistReason(reason) {
				if (auto rlGamerInfo = player->GetGamerInfo(); rlGamerInfo) {
					m_rid = rlGamerInfo->m_unk_gamer_handle.m_rockstar_id;
					m_platformData = rlGamerInfo->m_platform_data;
				}
			}
			uint64_t m_rid{};
			uint64_t m_platformData{};
			char const* m_name{};
			char const* m_blacklistReason{};
		};
		inline std::vector<blacklist> g_arrayOfBlacklistedPlayers{};
	}
}