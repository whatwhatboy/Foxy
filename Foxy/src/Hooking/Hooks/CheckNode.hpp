#pragma once
#include "Hooking/Hooking.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#define generateEncryptedRTTIHash(c) STRINGIFY(c)""_joaat
namespace base {
	constexpr uint32_t g_crashObjects[45] = {
		"prop_dummy_01"_joaat,
		"prop_dummy_car"_joaat,
		"prop_dummy_light"_joaat,
		"prop_dummy_plane"_joaat,
		"prop_distantcar_night"_joaat,
		"prop_distantcar_day"_joaat,
		"hei_bh1_08_details4_em_night"_joaat,
		"dt1_18_sq_night_slod"_joaat,
		"ss1_12_night_slod"_joaat,
		"h4_prop_bush_bgnvla_med_01"_joaat,
		"h4_prop_bush_bgnvla_lrg_01"_joaat,
		"h4_prop_bush_buddleia_low_01"_joaat,
		"h4_prop_bush_ear_aa"_joaat,
		"h4_prop_bush_ear_ab"_joaat,
		"h4_prop_bush_fern_low_01"_joaat,
		"h4_prop_bush_fern_tall_cc"_joaat,
		"h4_prop_bush_mang_ad"_joaat,
		"h4_prop_bush_mang_low_aa"_joaat,
		"h4_prop_bush_mang_low_ab"_joaat,
		"h4_prop_bush_seagrape_low_01"_joaat,
		"prop_h4_ground_cover"_joaat,
		"h4_prop_weed_groundcover_01"_joaat,
		"h4_prop_grass_med_01"_joaat,
		"h4_prop_grass_tropical_lush_01"_joaat,
		"h4_prop_grass_wiregrass_01"_joaat,
		"h4_prop_weed_01_plant"_joaat,
		"h4_prop_weed_01_row"_joaat,
		"urbanweeds02_l1"_joaat,
		"proc_forest_grass01"_joaat,
		"prop_small_bushyba"_joaat,
		"v_res_d_dildo_a"_joaat,
		"v_res_d_dildo_b"_joaat,
		"v_res_d_dildo_c"_joaat,
		"v_res_d_dildo_d"_joaat,
		"v_res_d_dildo_e"_joaat,
		"v_res_d_dildo_f"_joaat,
		"v_res_skateboard"_joaat,
		"prop_battery_01"_joaat,
		"prop_barbell_01"_joaat,
		"prop_barbell_02"_joaat,
		"prop_bandsaw_01"_joaat,
		"prop_bbq_3"_joaat,
		"v_med_curtainsnewcloth2"_joaat,
		"bh1_07_flagpoles"_joaat,
		92962485
	};
	inline bool isCrashObject(uint32_t model) {
		for (int32_t i{}; i != sizeof(g_crashObjects); ++i) {
			if (g_crashObjects[i] == model) {
				return false;
			}
		}
		return true;
	}
	template <typename t>
	t* getNodeFromObject(rage::netSyncNodeBase* node) {
		constexpr uint32_t hash = generateEncryptedRTTIHash(t);
		if (node->IsParentNode()) {
			for (auto child = node->m_first_child; child; child = child->m_next_sibling) {
				auto attachmentNode = getNodeFromObject<t>(child);
				if (attachmentNode != nullptr)
					return attachmentNode;
			}
		}
		else if (node->IsDataNode()) {
			if (node->GetTypeHash() == hash)
				return reinterpret_cast<t*>(node);
		}
		return nullptr;
	}
	inline bool isAttachmentInfinite(CPhysicalAttachDataNode* node, uint16_t objId) {
		if (auto attachedObj = pointers::g_getNetObject(util::network::object::mgr::get(), node->m_attached_to, false); attachedObj) {
			if (auto tree = attachedObj->GetSyncTree(); tree) {
				if (auto node = tree->m_sync_node; node) {		
					if (const auto attachedPhysicalDataNode = getNodeFromObject<CPhysicalAttachDataNode>(node); attachedPhysicalDataNode && attachedPhysicalDataNode->m_attached) {
						if (attachedPhysicalDataNode->m_attached_to == objId) {
							return true;
						}
						else {
							return isAttachmentInfinite(attachedPhysicalDataNode, objId);
						}
					}
				}
			}
		}
		return false;
	}
	inline bool isPedAttachmentInfinite(CPedAttachDataNode* node, uint16_t objId) {
		if (auto attachedObj = pointers::g_getNetObject(util::network::object::mgr::get(), node->m_attached_to, false); attachedObj) {
			if (auto tree = attachedObj->GetSyncTree(); tree) {
				if (auto node = tree->m_sync_node; node) {			
					if (const auto attachedPedDataNode = getNodeFromObject<CPedAttachDataNode>(node); attachedPedDataNode && attachedPedDataNode->m_attached) {
						if (attachedPedDataNode->m_attached_to == objId) {
							return true;
						}
						else {
							return isPedAttachmentInfinite(attachedPedDataNode, objId);
						}
					}
				}
			}
		}

		return false;
	}
	//Add array constrictors on the following data nodes, and look into how to exploit them
	// If the location is unknown, it's because it was found in a crash log of sorts
	//CDynamicEntityGameStateDataNode - Reason(s): OOB Array Read | Location(s): m_decors
	//CObjectGameStateDataNode - Reason(s): Array Read Overflow | Location(s): Unknown
	//CPedGameStateDataNode - Reason(s): Multiple OOB Array Reads | Location(s): m_weapon_component_something, m_weapon_component_hash, m_gadget_hash
	//CPedInventoryDataNode  - Reason(s): Multiple OOB Array Reads | Location(s): m_items, m_ammos, m_ammo_quantities, m_infinite_ammos
	//CPedTaskSpecificDataNode - Reason(s): Array Read Overflow | Location(s): m_task_data_buffer
	//CPickupCreationDataNode - Reason(s): OOB Array Read | Location(s): m_weapon_component
	//CVehicleControlDataNode - Reason(s): OOB Array Read | Location(s): Unknown
	//CVehicleGadgetDataNode - Reason(s): OOB Array Read | Location(s): m_gadget_data
	//CVehicleHealthDataNode - Reason(s): Multiple OOB Array Reads | Location(s): Unknown
	//CVehicleProximityMigrationDataNode - Reason(s): Multiple OOB Array Reads, Undefinied Behavior | Location(s): Unknown
	//CClonedTaskMoveScriptedInfo - Reason: Multiple OOB Array Reads | Location(s): Unknown
	inline bool checkNode(rage::netSyncNodeBase* node, CNetGamePlayer* sender, rage::netObject* object) {
		if (!isValidPtr(sender) || !isValidPtr(node)) {
			LOG_DEBUG("checkNode has a nullptr node or sender, this cannot possibly be good...")
			return false;
		}
		if (sender->IsConnected()) {
			if (node->IsParentNode()) {
				for (auto child = node->m_first_child; child; child = child->m_next_sibling) {
					checkNode(child, sender, object);
				}
			}
			else if (node->IsDataNode()) {
				LOG_DEBUG("'{}' from {}", node->GetTypeHash(), sender->GetName());
				switch (node->GetTypeHash()) {
				case generateEncrtypredRAGE_RTTI<"CPlayerAppearanceDataNode">(): {
					auto dataNode = reinterpret_cast<CPlayerAppearanceDataNode*>(node);
					if (dataNode->getDrawable(0) == 45 || dataNode->getDrawable(1) == 197) {
						if (protections::g_blockNextGenPedCrash.toggle) {
							auto togType = protections::g_blockNextGenPedCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (NG0) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
								return false;
						}
					}
					for (int32_t i{}; i != protections::m_crashModels.size(); ++i) {
						if (dataNode->m_model_hash == protections::m_crashModels[i].hash) {
							if (protections::g_blockInvalidPlayerModelCrash.toggle) {
								auto togType = protections::g_blockInvalidPlayerModelCrash.toggleType;
								if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
									g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (IPM{}) from {}"), sender->GetName(), i);
								if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
									return false;
							}
						}
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CVehicleCreationDataNode">(): {
					auto dataNode = reinterpret_cast<CVehicleCreationDataNode*>(node);
					int crashType{};
					switch (dataNode->m_model) {
					case "arbitergt"_joaat: crashType = 1; break;
					case "astron2"_joaat: crashType = 2; break;
					case "cyclone2"_joaat: crashType = 3; break;
					case "ignus2"_joaat: crashType = 4; break;
					case "s95"_joaat: crashType = 5; break;
					}
					if (protections::g_blockNextGenVehicleCrash.toggle && crashType) {
						auto togType = protections::g_blockNextGenVehicleCrash.toggleType;
						if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
							g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (NG{}) from {}"), crashType, sender->GetName());
						if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
							return false;
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CPedCreationDataNode">(): {
					auto dataNode = reinterpret_cast<CPedCreationDataNode*>(node);
					if (dataNode->m_has_prop && isCrashObject(dataNode->m_prop_model)) {
						if (protections::g_blockInvalidPedPropModelCrash.toggle) {
							auto togType = protections::g_blockInvalidPedPropModelCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (IPM0) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
								return false;
						}
						return true;
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CPhysicalAttachDataNode">(): {
					auto dataNode = reinterpret_cast<CPhysicalAttachDataNode*>(node);
					auto modelHash = object->GetGameObject() ? object->GetGameObject()->m_model_info->m_model : 0;
					if (dataNode->m_attached && dataNode->m_attached_to == object->m_object_id && (modelHash != "hauler2"_joaat && modelHash != "phantom3"_joaat)) {
						if (protections::g_blockInvalidAttachmentCrash.toggle) {
							auto togType = protections::g_blockInvalidAttachmentCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (A2) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
								dataNode->m_attached = false;
								dataNode->m_attached_to = NULL;
								return false;
							}
						}
					}
					if (isAttachmentInfinite(dataNode, object->m_object_id)) {
						if (protections::g_blockInvalidAttachmentCrash.toggle) {
							auto togType = protections::g_blockInvalidAttachmentCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (A3) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
								dataNode->m_attached = false;
								dataNode->m_attached_to = NULL;
								return false;
							}
						}
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CPedAttachDataNode">(): {
					auto dataNode = reinterpret_cast<CPedAttachDataNode*>(node);
					auto modelHash = object->GetGameObject() ? object->GetGameObject()->m_model_info->m_model : 0;
					if (dataNode->m_attached && dataNode->m_attached_to == object->m_object_id) {
						if (protections::g_blockInvalidAttachmentCrash.toggle) {
							auto togType = protections::g_blockInvalidAttachmentCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (A4) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
								dataNode->m_attached = false;
								dataNode->m_attached_to = NULL;
								return false;
							}
						}
					}
					if (isPedAttachmentInfinite(dataNode, object->m_object_id)) {
						if (protections::g_blockInvalidAttachmentCrash.toggle) {
							auto togType = protections::g_blockInvalidAttachmentCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (A5) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
								dataNode->m_attached = false;
								dataNode->m_attached_to = NULL;
								return false;
							}
						}
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CSectorDataNode">(): {
					auto dataNode = reinterpret_cast<CSectorDataNode*>(node);
					auto pos_x = dataNode->m_pos_x;
					auto pos_y = dataNode->m_pos_y;
					auto pos_z = dataNode->m_pos_z;
					if (dataNode->pad_0000[64] && ((pos_x > 711.f && pos_y > 711.f && pos_z > 711.f) || (pos_x == 712 && pos_y == 712 && pos_z == 712))) {
						if (protections::g_blockHostCrash.toggle) {
							auto togType = protections::g_blockHostCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (S0) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
								return false;
						}
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CDoorCreationDataNode">(): {
					auto dataNode = reinterpret_cast<CDoorCreationDataNode*>(node);
					if (isCrashObject(dataNode->m_model)) {
						if (protections::g_blockInvalidDoorModelCrash.toggle) {
							auto togType = protections::g_blockInvalidDoorModelCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (IM1) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
								return false;
						}
					}
				} break;
				case generateEncrtypredRAGE_RTTI<"CPickupCreationDataNode">(): {
					auto dataNode = reinterpret_cast<CPickupCreationDataNode*>(node);
					if (isCrashObject(dataNode->m_custom_model)) {
						if (protections::g_blockInvalidDoorModelCrash.toggle) {
							auto togType = protections::g_blockInvalidDoorModelCrash.toggleType;
							if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
								g_dxUiManager.sendNotify(xorstr_("Crash Protections"), xorstr_("Crash (IM2) from {}"), sender->GetName());
							if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock)
								return false;
						}
					}
					return dataNode->ensureArraySecurity();
				} break;
				case generateEncrtypredRAGE_RTTI<"CDynamicEntityGameStateDataNode">(): {
					auto dataNode = reinterpret_cast<CDynamicEntityGameStateDataNode*>(node);
					return dataNode->ensureArraySecurity();
				} break;
				case generateEncrtypredRAGE_RTTI<"CPedGameStateDataNode">(): {
					auto dataNode = reinterpret_cast<CPedGameStateDataNode*>(node);
					return dataNode->ensureArraySecurity();
				} break;
				case generateEncrtypredRAGE_RTTI<"CPedInventoryDataNode">(): {
					auto dataNode = reinterpret_cast<CPedInventoryDataNode*>(node);
					return dataNode->ensureArraySecurity();
				} break;
				case generateEncrtypredRAGE_RTTI<"CPedTaskSpecificDataNode">(): {
					auto dataNode = reinterpret_cast<CPedTaskSpecificDataNode*>(node);
					return dataNode->ensureArraySecurity();
				} break;
				case generateEncrtypredRAGE_RTTI<"CVehicleGadgetDataNode">(): {
					auto dataNode = reinterpret_cast<CVehicleGadgetDataNode*>(node);
					return dataNode->ensureArraySecurity();
				} break;
				}
			}
		}
		return true;
	}
}