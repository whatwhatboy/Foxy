#pragma once
#include "Pch/Common.hpp"
#include "Core/Logger.hpp"
#include <minhook.h>

namespace base {
	class MinHook {
	public:
		MinHook() { MH_Initialize(); }
		~MinHook() { MH_Uninitialize(); }
	public:
		template <typename T>
		static bool Create(T Target, T Detour, T* Originial) {
			MH_STATUS Status{ MH_CreateHook(Target, LPVOID(Detour), (LPVOID*)Originial) };
			if (Status == MH_STATUS::MH_OK) {
				return true;
			}
			if (Status != MH_STATUS::MH_ERROR_NOT_INITIALIZED && Status != MH_STATUS::MH_ERROR_UNSUPPORTED_FUNCTION && Status != MH_STATUS::MH_ERROR_MEMORY_ALLOC && Status != MH_STATUS::MH_ERROR_MEMORY_PROTECT) {
				if (std::optional<LPVOID> CorrectedTarget{ GetCorrectedTarget<T>(Target) }; CorrectedTarget.has_value()) {
					Status = MH_CreateHook(CorrectedTarget.value(), LPVOID(Detour), (LPVOID*)Originial);
					return Status == MH_STATUS::MH_OK;
				}
			}
			return false;
		}
		template <typename T>
		static bool Destroy(T Target) {
			return MH_RemoveHook(LPVOID(Target)) == MH_STATUS::MH_OK;
		}
		template <typename T>
		static bool QueueEnable(T Target) {
			return MH_QueueEnableHook(LPVOID(Target)) == MH_STATUS::MH_OK;
		}
		template <typename T>
		static bool QueueDisable(T Target) {
			return MH_QueueDisableHook(LPVOID(Target)) == MH_STATUS::MH_OK;
		}
		static bool Apply() {
			return MH_ApplyQueued() == MH_STATUS::MH_OK;
		}
	public:
		template <typename T>
		static std::optional<LPVOID> GetCorrectedTarget(T Target) {
			uint8_t* Bytes{ decltype(Bytes)(Target) };
			for (int8_t i{}; i != 0xF; ++i) {
				if (Bytes[i] == 0xE8) {
					return *(LPVOID*)(&Bytes[i]);
				}
			}
			return std::nullopt;
		}
	};
	class detour {
	public:
		detour(char const* name, void* ptr, void* dtr, bool hook = false) : m_name(name), m_ptr(ptr), m_dtr(dtr), m_hook(hook) {
			if (!m_hook) {
				if (!MinHook::Create(m_ptr, m_dtr, &m_og))
					LOG(Detours, "Failed to create hook {}", m_name);
			}
		}
		~detour() {
			if (!m_hook) {
				if (!MinHook::Destroy(m_ptr))
					LOG(Detours, "Failed to remove hook {}", m_name);
			}
		}
	public:
		void enable() {
			if (!m_hook) {
				if (!MinHook::QueueEnable(m_ptr))
					LOG(Detours, "Failed to enable hook {}", m_name);
			}
		}
		void disable() {
			if (!m_hook) {
				if (!MinHook::QueueDisable(m_ptr))
					LOG(Detours, "Failed to disable hook {}", m_name);
			}
		}
	public:
		inline static void enableAll() {
			MH_QueueEnableHook(MH_ALL_HOOKS);
		}
		inline static void disableAll() {
			MH_QueueDisableHook(MH_ALL_HOOKS);
		}
	public:
		template <typename t>
		t getOg() {
			return t(m_og);
		}
	public:
		char const* m_name;
		void* m_dtr;
		void* m_ptr;
		void* m_og;
		bool m_hook;
	};
}
