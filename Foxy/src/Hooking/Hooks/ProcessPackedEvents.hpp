#include "Hooking/Hooking.hpp"
#include "R.A.G.Engine/CScriptedGameEvent.hpp"
#include "R.A.G.Engine/CNetworkIncrementStatEvent.hpp"
#include "Util/Util.hpp"

namespace base {
	inline auto g_scriptedGameEvent_Callback = [](CScriptedGameEvent* sge, CNetGamePlayer* sender) -> bool {
		auto args = sge->m_args;
		if (protections::g_logScriptEvents)
			for (int i{}; i != sge->m_args_size; ++i)
				g_logger.get()->log(eLogColor::Grey, xorstr_("Script Events"), xorstr_("Arg[{}]: {}"), i, args[i]);
		//if (!g_hookQueue.tickSge(sender->m_player_id, args, sge->m_args_size))
		//	return true;
		for (auto& evnt : protections::m_scriptEvents) {
			if (args[0] == evnt.hash && evnt.toggle.toggle) {
				auto togType = evnt.toggle.toggleType;
				if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
					g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("{} Event from {}"), evnt.name, sender->GetName());
				if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
					return false;
			}
		}
		return true;
	};
	inline auto g_incrementStatEvent_Callback = [](CNetworkIncrementStatEvent* ise, CNetGamePlayer* sender) -> bool {
		for (auto& evnt : protections::m_reportHashes) {
			if (ise->m_stat == evnt.hash && evnt.toggle.toggle) {
				auto togType = evnt.toggle.toggleType;
				if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
					g_dxUiManager.sendNotify(xorstr_("Report Protections"), xorstr_("{} Report from {}"), evnt.hash, sender->GetName());
				if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
					return false;
			}
		}
		return true;
	};
	inline auto g_blockCMsgPackedEvents_Callback = [](rage::netEventMgr* eventMgr, CNetGamePlayer* source, CNetGamePlayer* target, int32_t idx, int32_t handledBitset) {
		pointers::g_sendEventAck(eventMgr, source, target, idx, handledBitset);
		return;
	};
	inline auto g_proccessPackedEvents_GameAck_Callback = [](rage::netEventMgr* eventMgr, std::string reason) {
		uint64_t netEventMgrAddr{ uintptr_t(eventMgr) };
		uint64_t** vtable{ (uint64_t**)(netEventMgrAddr + 0x3E9F8) };
		auto fn{ (*(void(**)(uint64_t, const char*, const char*))(**vtable + 0x30i64)) };
		fn((uint64_t)*vtable, "IGNORED", reason.c_str());
	};
	inline auto g_proccessPackedEvents_EventHandle_Callback = [](rage::netEventMgr* eventMgr, CNetGamePlayer* source, CNetGamePlayer* target, uint16_t id, int32_t idx, int32_t handledBitset, int32_t bufferSize, rage::datBitBuffer* buffer) {
		uint64_t netEventMgrAddr{ uintptr_t(eventMgr) };
		uint64_t addr = *(uint64_t*)(netEventMgrAddr + 0x3E9F0);
		const char* eventName = "Unknown Event";
		if (const char* tmpName = *(const char**)(netEventMgrAddr + 8ui64 * id + 0x3B6B0); tmpName)
			eventName = tmpName;
		const char* tmpEventName = eventName;
		const char* format = "%s_%d (%d)";
		const char* eventHandler = "RECEIVED_EVENT";
		auto handler = *(void(**)(rage::datBitBuffer*, CNetGamePlayer*, CNetGamePlayer*, uint64_t, const char*, const char*, const char*, int, int))(netEventMgrAddr + 8 * id + 0x3B3D0);
		*((uint32_t*)&(format)) = handledBitset;
		*((uint32_t*)&(eventHandler)) = idx;
		handler(buffer, source, target, bufferSize, eventHandler, format, tmpEventName, idx, handledBitset);
	};
	inline auto g_recievedEvent = [](rage::netEventMgr* eventMgr, CNetGamePlayer* source, CNetGamePlayer* target, uint16_t id, int32_t idx, int32_t handledBitset, int32_t bufferSize, rage::datBitBuffer* buffer) {
		if (id < 0x5Au) {
			if (target == util::player::g_networkHandle && (handledBitset << 24) - (*(uint32_t*)(uintptr_t(eventMgr) + 4 * (source->m_player_id + 70i64 * idx) + 0x458) << 24) <= 0) {
				return g_proccessPackedEvents_GameAck_Callback(eventMgr, "Handled already");
			}
			if (source->m_player_id == 0xFF) {
				return g_proccessPackedEvents_GameAck_Callback(eventMgr, "Player does not have a valid physical player index");
			}
			return g_proccessPackedEvents_EventHandle_Callback(eventMgr, source, target, id, idx, handledBitset, bufferSize, buffer);
		}
	};
	inline auto g_processPackedEvents = [&](rage::netEventMgr* eventMgr, CNetGamePlayer* source, CNetGamePlayer* target, uint16_t id, int32_t idx, int32_t handledBitset, int32_t bufferSize, rage::datBitBuffer* buffer) {
		uint64_t netEventMgrAddr{ uintptr_t(eventMgr) };
		const char* eventName{ *(const char**)(netEventMgrAddr + 8ui64 * id + 0x3B6B0) };
		if (eventName == nullptr || source == nullptr || source->m_player_id < 0 || source->m_player_id >= 32)
			return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
		switch (static_cast<eNetworkEvents>(id)) {
		case eNetworkEvents::CScriptedGameEvent: {
			auto sge = std::make_unique<CScriptedGameEvent>();
			buffer->ReadDword(&sge->m_args_size, 0x20);
			if (sge->m_args_size - 1 <= 0x1AF)
				buffer->ReadArray(&sge->m_args, 8 * sge->m_args_size);
			if (!g_scriptedGameEvent_Callback(sge.get(), source))
				return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CNetworkIncrementStatEvent: {
			auto ise = std::make_unique<CNetworkIncrementStatEvent>();
			buffer->ReadDword(&ise->m_stat, 0x20);
			buffer->ReadDword(&ise->m_amount, 0x20);
			if (!g_incrementStatEvent_Callback(ise.get(), source))
				return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CScriptEntityStateChangeEvent: {
			uint16_t entity{};
			uint32_t type{};
			uint32_t unk{};
			buffer->ReadWord(&entity, 0xD);
			buffer->ReadDword(&type, 0x4);
			buffer->ReadDword(&unk, 0x20);
			if (type == 6) {
				uint16_t unk2{};
				uint32_t action{};
				buffer->ReadWord(&unk2, 0xD);
				buffer->ReadDword(&action, 0x8);
				if (protections::g_blockVehicleTaskCrash.toggle) {
					auto togType = protections::g_blockVehicleTaskCrash.toggleType;
					if (action >= 15 && action <= 18) {
						if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
							g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (T{}) from {}"), type, source->GetName());
						if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
							return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
					}
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CExplosionEvent: {
			uint16_t explosionOwner{};
			uint32_t explosionFlags{};
			int32_t explosionType{};
			buffer->Seek(0x1D);
			buffer->ReadWord(&explosionOwner, 0xD);
			buffer->Seek(0x37);
			buffer->ReadInt32(&explosionType, 0x8);
			buffer->Seek(0x95);
			buffer->ReadDword(&explosionFlags, 0x20);
			if (explosionOwner != NULL) {
				if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), explosionOwner, FALSE); netObj) {
					if (auto pedOwnerId = netObj->m_owner_id; pedOwnerId != NULL && protections::g_blockExplosions.toggle) {
						auto togType = protections::g_blockExplosions.toggleType;
						if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock) {
							if (pedOwnerId != source->m_player_id)
								g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("{} (T{}) from: {}"), explosionType, source->GetName(), (explosionFlags & 0xB) ? xorstr_("Blamed explosion") : xorstr_("Explosion"));
						}
						if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
							return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
					}
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CNetworkClearPedTasksEvent: {
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
				uint16_t networkId{};
				buffer->ReadWord(&networkId, 0xD);
				if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), networkId, FALSE); netObj) {
					if (netObj->m_owner_id == PLAYER::PLAYER_ID() && protections::g_blockFreeze.toggle) {
						auto togType = protections::g_blockFreeze.toggleType;
						if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
							g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Freeze from {}"), source->GetName());
						if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
							return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
					}
				}
				buffer->Seek(0);
			}
		} break;
		case eNetworkEvents::CRequestControlEvent: {
			uint16_t networkId{};
			buffer->ReadWord(&networkId, 0xD);
			if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), networkId, FALSE); netObj) {
				if (netObj->m_owner_id == PLAYER::PLAYER_ID() && protections::g_blockRequestControl.toggle) {
					auto togType = protections::g_blockRequestControl.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Request control from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CGiveControlEvent: {
			uint16_t networkId{};
			buffer->ReadWord(&networkId, 0xD);
			buffer->Read<uint32_t>(0x20);
			auto count = buffer->Read<uint32_t>(2);
			buffer->Read<uint32_t>(1);
			if (count > 3)
				count = 3;
			for (uint8_t i{}; i != count; ++i) {
				buffer->Read<uint32_t>(0xD);
				auto syncTreeType = buffer->Read<uint32_t>(4);
				buffer->Read<uint32_t>(3);
				if (syncTreeType > 13 && protections::g_blockGiveControlCrash.toggle) {
					auto togType = protections::g_blockGiveControlCrash.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (GC) from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CGiveWeaponEvent: {
			uint16_t networkId{};
			buffer->ReadWord(&networkId, 0xD);
			if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), networkId, FALSE); netObj) {
				if (netObj->m_owner_id == PLAYER::PLAYER_ID() && protections::g_blockGiveWeapon.toggle) {
					auto togType = protections::g_blockGiveWeapon.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Give weapon from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CRemoveWeaponEvent: {
			uint16_t networkId{};
			buffer->ReadWord(&networkId, 0xD);
			if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), networkId, FALSE); netObj) {
				if (netObj->m_owner_id == PLAYER::PLAYER_ID() && protections::g_blockRemoveWeapon.toggle) {
					auto togType = protections::g_blockRemoveWeapon.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Remove weapon from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CRemoveAllWeaponsEvent: {
			uint16_t networkId{};
			buffer->ReadWord(&networkId, 0xD);
			if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), networkId, FALSE); netObj) {
				if (netObj->m_owner_id == PLAYER::PLAYER_ID() && protections::g_blockRemoveAllWeapons.toggle) {
					auto togType = protections::g_blockRemoveAllWeapons.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Remove all weapons from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CKickVotesEvent: {
			uint32_t bitset{};
			buffer->ReadDword(&bitset, 0x20);
			if (target->m_player_id == PLAYER::PLAYER_ID() && (bitset & (1 << target->m_player_id))) {
				if (protections::g_blockVoteKicks.toggle) {
					auto togType = protections::g_blockVoteKicks.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Vote kick from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		case eNetworkEvents::CAlterWantedLevelEvent: {
			uint16_t networkId{};
			buffer->ReadWord(&networkId, 0xD);
			if (auto netObj = pointers::g_getNetObject(util::network::object::mgr::get(), networkId, FALSE); netObj) {
				if (netObj->m_owner_id == PLAYER::PLAYER_ID() && protections::g_blockAlterWantedLvl.toggle) {
					auto togType = protections::g_blockAlterWantedLvl.toggleType;
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Wanted level alter from {}"), source->GetName());
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
						return g_blockCMsgPackedEvents_Callback(eventMgr, source, target, idx, handledBitset);
				}
			}
			buffer->Seek(0);
		} break;
		}
		g_recievedEvent(eventMgr, source, target, id, idx, handledBitset, bufferSize, buffer);
	};
}