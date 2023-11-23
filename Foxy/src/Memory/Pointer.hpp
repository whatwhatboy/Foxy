#pragma once
#include "Pch/Common.hpp"
#include "Memory/Memory.hpp"

namespace base::memory {
	enum class ePointerThreadState : int8_t {
		Invalid = -1,
		Killed,
		Paused,
		Active
	};
	class pointer {
	public:
		pointer(hmod hmod, char const* name, char const* sig, char const* parse) : m_memory(memory(hmod, parse, name, sig)) {}
		pointer() = default;
	public:
		auto thr() {
			return std::thread([&]() {
				if (m_state == ePointerThreadState::Killed)
					return;
				m_state = ePointerThreadState::Active;
				m_memory.parse();
				m_state = ePointerThreadState::Killed;
			});
		}
	public:
		bool valid() {
			return m_state == ePointerThreadState::Killed;
		}
		uint64_t relative() {
			return *m_memory - m_memory.m_module.m_begin.as<uint64_t>();
		}
		std::string name() {
			return m_memory.m_name;
		}
		uint64_t operator()() {
			return *m_memory;
		}
	public:
		memory m_memory{};
		ePointerThreadState m_state{ ePointerThreadState::Invalid };
	};
}