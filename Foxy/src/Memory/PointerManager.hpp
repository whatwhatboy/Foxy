#pragma once
#include "Pch/Common.hpp"
#include "Memory/Pointer.hpp"
#include "Memory/Util.hpp"

namespace base::memory {
	class pointerManager {
	public:
		pointerManager(hmod hmod) : m_hmod(hmod) {}
		pointerManager() : pointerManager(hmod()) {}
		void add(char const* name, char const* sig, char const* parse = "") {
			m_pointers.insert(std::make_pair(name, pointer(m_hmod, name, sig, parse)));
		}
		template <typename t = void*>
		t operator[](char const* name) {
			return get<t>(name);
		}

		template <typename t>
		t get(char const* name) {
			for (auto& p : m_pointers) {
				auto& ptr = p.second;
				if (!ptr.name().compare(name)) {
					return t(ptr());
				}
			}
			return (t)nullptr;
		}
	public:
		void run() {
			if (m_runCalled)
				return;
			else
				m_runCalled = true;
			for (auto& p : m_pointers) {
				auto& ptr = p.second;
				ptr.thr().detach();
			}
		}
	public:
		bool allThreadsFinished() {
			if (!m_runCalled)
				run();
			for (auto& p : m_pointers) {
				auto& ptr = p.second;
				if (!ptr.valid())
					return false;
			}
			return true;
		}
		void print() {
			for (auto& p : m_pointers) {
				auto& ptr = p.second;
				LOG_DEBUG("{} {}+0x{:X}", ptr.name(), ptr.m_memory.m_module.m_moduleName, ptr.relative());
			}
		}
	public:
		auto counts() {
			return std::make_pair(util::g_foundSigCount, util::g_totalSigCount);
		}
		void clear() {
			m_pointers.clear();
		}
	public:
		hmod m_hmod{};
		bool m_runCalled{};
		std::map<char const*, pointer> m_pointers{};
	};
}