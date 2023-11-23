#pragma once
#include "Hooking/HookingMethods/ScriptHook.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberMgr.hpp"
#include "NativesDetours.hpp"

namespace base {
	typedef std::pair<rage::scrNativeHash, rage::Cmd> nativeDetour;
	class nativeHooks final {
	public:
		nativeHooks() {
			addNativeDetour("all_scripts"_joaat, 0x6D0DE6A7B5DA71F8, natives::PLAYER_D::GET_PLAYER_NAME);

			addNativeDetour("freemode"_joaat, 0x95914459A87EBA28, natives::NETWORK_D::NETWORK_BAIL);
			addNativeDetour("maintransition"_joaat, 0x95914459A87EBA28, natives::NETWORK_D::NETWORK_BAIL);
			addNativeDetour("maintransition"_joaat, 0x6F3D4ED9BEE4E61D, natives::NETWORK_D::NETWORK_SESSION_HOST);
			addNativeDetour("all_scripts"_joaat, 0x580CE4438479CC61, natives::NETWORK_D::NETWORK_CAN_BAIL);

			addNativeDetour("all_scripts"_joaat, 0x812595A0644CE1DE, natives::DLC_D::IS_DLC_PRESENT);

			addNativeDetour("all_scripts"_joaat, 0xD4E8E24955024033, natives::CAM_D::DO_SCREEN_FADE_IN);
			addNativeDetour("all_scripts"_joaat, 0x891B5B39AC6302AF, natives::CAM_D::DO_SCREEN_FADE_OUT);

			searchForValidScriptsAndHook();
		}
		~nativeHooks() = default;
	public:
		void searchForValidScriptsAndHook() {
			for (auto& entry : *pointers::g_scrProgramTbl)
				if (entry.m_program && entry.m_program->is_valid())
					hookProgram(entry.m_program);
		}
		void addNativeDetour(rage::joaat_t script_hash, rage::scrNativeHash hash, rage::Cmd detour) {
			if (const auto& it = m_nativeRegistrations.find(script_hash); it != m_nativeRegistrations.end()) {
				it->second.emplace_back(hash, detour);
				return;
			}
			m_nativeRegistrations.emplace(script_hash, std::vector<nativeDetour>({ { hash, detour } }));
		}
		void hookProgram(rage::scrProgram* scr) {
			static std::unordered_map<rage::scrNativeHash, rage::Cmd> nativeReplacements;
			const auto scrHash = scr->m_name_hash;
			if (const auto& pair = m_nativeRegistrations.find("all_scripts"_joaat); pair != m_nativeRegistrations.end()) {
				for (const auto& reg : pair->second)
					nativeReplacements.insert(reg);
			}
			if (const auto& pair = m_nativeRegistrations.find(scrHash); pair != m_nativeRegistrations.end()) {
				for (const auto& reg : pair->second)
					nativeReplacements.insert(reg);
			}
			LOG_DEBUG("Hooked program {}", scr->m_name);
			if (!nativeReplacements.empty()) {
				m_scriptHooks.emplace(scr, std::make_unique<scriptHook>(scrHash, nativeReplacements, [=](rage::scrProgram* program) {
					if (auto it = m_scriptHooks.find(program); it != m_scriptHooks.end()) {
						m_scriptHooks.erase(it);
					}
				}));
			}
		}
	public:
		std::map<rage::joaat_t, std::vector<nativeDetour>> m_nativeRegistrations;
		std::map<rage::scrProgram*, std::unique_ptr<scriptHook>> m_scriptHooks;
	};
	inline std::unique_ptr<nativeHooks> g_nativeHooks;
}