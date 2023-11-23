#pragma once
#include "Pch/Common.hpp"
#include "Memory/ScriptPatch.hpp"
#include "R.A.G.Engine/ScrProgram.hpp"
#include "Features/Features.hpp"

namespace base {
	class scriptPatcher {
	public:
		scriptPatcher() {
			//add({ "freemode"_joaat, "5D ? ? ? 76 57 ? ? 5D ? ? ? 76", 0, { 0x2E, 0x00, 0x00 }, nullptr }); //End session kick prot
			//add({ "freemode"_joaat, "2D 01 09 00 00 5D ? ? ? 56 ? ? 2E", 5, { 0x2E, 0x01, 0x00 }, nullptr }); //Disable death when spectating
			//add({ "freemode"_joaat, "71 2E ? ? 55 ? ? 61 ? ? ? 47 ? ? 63", 0, { 0x72 }, nullptr }); //Load island even if stranded animal IPL choice is not set
			//add({ "freemode"_joaat, "2D 00 07 00 00 7B", 5, { 0x2E, 0x00, 0x00 }, nullptr }); //Disable population load balancing
			//add({ "freemode"_joaat, "5D ? ? ? 56 ? ? 72 39 05 38 04 2C ? ? ? 58", 0, { 0x2B, 0x2B, 0x2B, 0x00, 0x55 }, &features::invisibilityBool });

			//add({ "shop_controller"_joaat, "2D 01 04 00 00 2C ? ? ? 56 ? ? 71", 5, { 0x71, 0x2E, 0x01, 0x01 }, nullptr }); //Despawn bypass
			//add({ "shop_controller"_joaat, "38 00 5D ? ? ? 38 00 5D ? ? ? 38 00 41", 0, std::vector<uint8_t>(12, 0x00), nullptr }); //Godmode/invisibility detection bypass
			//for (auto& entry : *pointers::g_scrProgramTbl) {
			//	if (auto& program{ entry.m_program }; program && program->is_valid()) {
			//		onLoad(program);
			//	}
			//}
		}
		~scriptPatcher() {
			m_data.clear();
			m_patches.clear();
		}
	public:
		void onLoad(rage::scrProgram* program) {
			if (!getData(program->m_name_hash) && hasPatches(program->m_name_hash)) {
				createData(program);
				updateScript(program->m_name_hash);
			}
		}
	public:
		void add(memory::script::patch patch) {
			m_patches.push_back(patch);
		}
		void updateScript(rage::joaat_t script) {
			for (auto& p : m_patches) {
				if (auto data{ getData(p.script) }; data && p.script == script) {
					p.update(data);
				}
			}
		}
		void update() {
			for (auto& p : m_patches) {
				if (auto data{ getData(p.script) }; data) {
					p.update(data);
				}
			}
		}
	public:
		void createData(rage::scrProgram* program) {
			auto pages = new uint8_t * [program->get_num_code_pages()];
			for (uint32_t i{}; i != program->get_num_code_pages(); ++i) {
				pages[i] = new uint8_t[program->get_code_page_size(i)];
				memcpy(pages[i], program->get_code_page(i), program->get_code_page_size(i));
			}
			m_data.emplace(program->m_name_hash, std::make_unique<memory::script::data>(program->m_code_size, pages, program->get_num_code_pages()));
		}
	public:
		bool hasPatches(rage::joaat_t script) {
			for (auto& p : m_patches) {
				if (p.script == script) {
					return true;
				}
			}
			return false;
		}
		uint8_t** getBytes(rage::joaat_t script) {
			if (auto data{ getData(script) }; data)
				return data->bytes;
			return nullptr;
		}
		memory::script::data* getData(rage::joaat_t script) {
			for (auto& p : m_data) {
				auto& scr{ p.first };
				auto& data{ p.second };
				if (scr == script) {
					return data.get();
				}
			}
			return nullptr;
		}
	public:
		std::vector<memory::script::patch> m_patches{};
		std::map<rage::joaat_t, std::unique_ptr<memory::script::data>> m_data{};
	};
	inline std::unique_ptr<scriptPatcher> g_scriptPatcher{};
}