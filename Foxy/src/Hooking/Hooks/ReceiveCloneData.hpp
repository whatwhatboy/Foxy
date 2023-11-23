#pragma once
#include "Hooking/Hooking.hpp"
#include "Util/Util.hpp"
#include "CheckNode.hpp"

namespace base {
	inline auto g_receiveCloneData = [](CNetworkObjectMgr* objMgr, CNetGamePlayer* sender, uint16_t syncType, uint16_t objId, rage::datBitBuffer* buffer) -> int {
		if (auto syncTree = pointers::g_getSyncTree(objMgr, syncType); syncTree && util::network::isSessionActive()) {
			if (auto netObj = pointers::g_getNetObject(objMgr, objId, true); netObj) {
				if (syncType < 0 || syncType > 14) {
					if (protections::g_blockOutOfRangeSyncCrash.toggle) {
						auto togType = protections::g_blockOutOfRangeSyncCrash.toggleType;
						if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
							g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (S{}) from {}"), syncType, sender->GetName());
						if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
							return 3;
					}
				}
				else if (netObj->m_object_type != syncType) {
					if (protections::g_blockSyncMismatchCrash.toggle) {
						auto togType = protections::g_blockSyncMismatchCrash.toggleType;
						if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
							g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (SM{}) from {}"), syncType, sender->GetName());
						if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
							return 3;
					}
				}
				else if (auto gameObj = netObj->GetGameObject()) {
					if (const auto modelInfo = gameObj->m_model_info) {
						if (const auto model = util::vehicle::getModelData(modelInfo->m_model); model) {
							if (modelInfo->m_model_type != model->m_model_type)
								return false;
							//else if (util::isVehicleSyncType(syncType) && reinterpret_cast<rage::CVehicleModelInfo*>(modelInfo)->m_vehicle_type != reinterpret_cast<rage::CVehicleModelInfo*>(model)->m_vehicle_type) {
								//return 3;
							//}
						}
						for (auto& mdl : protections::m_crashModels) {
							if (modelInfo->m_model == mdl.hash && mdl.toggle.toggle) {
								auto togType = mdl.toggle.toggleType;
								if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
									g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("{} crash from {}"), mdl.name, sender->GetName());
								if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
									return 3;
							}
						}
					}
				}
			}
		}
		return 1;
	};
}
