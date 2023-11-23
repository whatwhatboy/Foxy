#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#include "R.A.G.Engine/ScrProgram.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "VMTHook.hpp"
#include "R.A.G.Engine/Natives/Invoker/Invoker.hpp"
#include "Memory/Patterns/Pointers.hpp"

namespace base {
	class scriptHook;
	static std::unordered_map<rage::scrProgram*, scriptHook*> g_scriptMap;
	class scriptHook {
	public:
		explicit scriptHook(uint32_t scriptHash, std::unordered_map<uint64_t, rage::Cmd> nativeReplacements, std::function<void(rage::scrProgram*)> callback) : m_scriptHash(scriptHash), m_nativeReplacements(std::move(nativeReplacements)) {
			m_callback = callback;
			ensure();
		}
		~scriptHook() {
			if (m_program)
				for (auto [hash, handlerPtr] : m_nativeHandlerPtrs)
					*handlerPtr = g_invoker.m_cache[hash];
			if (m_vftHook) {
				m_vftHook->disable();
				g_scriptMap.erase(m_program);
			}
		}
	public:
		void ensure() {
			if (m_vftHook)
				return;
			if (auto program = pointers::g_scrProgramTbl->find_script(m_scriptHash)) {
				if (program->is_valid()) {
					hookInstance(program);
				}
			}
		}
		void hookInstance(rage::scrProgram* program) {
			m_program = program;
			g_scriptMap.emplace(m_program, this);
			m_vftHook = std::make_unique<hookVFT>(m_program, 9);
			m_vftHook->hook(&scrprogramDtor, 6);
			m_vftHook->enable();
			for (auto [hash, handler] : m_nativeReplacements) {
				auto ogHandler = g_invoker.m_cache[hash];
				if (!ogHandler)
					continue;
				auto handlerPtr = m_program->get_native(ogHandler);
				if (!handlerPtr)
					continue;
				m_nativeHandlerPtrs.emplace(hash, reinterpret_cast<rage::Cmd*>(handlerPtr));
				*handlerPtr = handler;
			}
		}
		static void scrprogramDtor(rage::scrProgram* this_, bool freeMem) {
			if (auto it = g_scriptMap.find(this_); it != g_scriptMap.end()) {
				auto hook = it->second;
				auto ogFunc = hook->m_vftHook->getOg<decltype(&scrprogramDtor)>(0);
				hook->m_program = nullptr;
				g_scriptMap.erase(it);
				if (m_callback)
					m_callback(this_);
				hook->m_vftHook->disable();
				ogFunc(this_, freeMem);
			}
		}
	public:
		uint32_t m_scriptHash;
		rage::scrProgram* m_program;
		std::unique_ptr<hookVFT> m_vftHook;
	public:
		std::unordered_map<uint64_t, rage::Cmd> m_nativeReplacements;
		std::unordered_map<uint64_t, rage::Cmd*> m_nativeHandlerPtrs;
	public:
		inline static std::function<void(rage::scrProgram*)> m_callback;
	};
}