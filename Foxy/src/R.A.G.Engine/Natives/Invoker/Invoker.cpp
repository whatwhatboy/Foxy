#include "Invoker.hpp"
#include "../Crossmap.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "Core/Logger.hpp"
#include "Stack/CallSpoof.hpp"

namespace base {
	void invoker::cache() {
		if (auto regTbl = pointers::g_nativeRegTbl; regTbl) {
			for (auto& entry : g_crossmap) {
				m_cache.emplace(entry.hash, pointers::g_getScrCommand(regTbl, entry.translatedHash));
			}
		}
	}

	void invoker::begin() {
		m_context.reset();
	}

	void invoker::end(rage::scrNativeHash hash) {
		if (auto val = m_cache.find(hash); val != m_cache.end()) {
			rage::Cmd& cmd{ val->second };
			if (!cmd) {
				LOG(Fatal, "Failed to invoke native 0x{:X}", hash)
				return;
			}
			spoofCall(pointers::g_jmpRbxRegister, cmd, dynamic_cast<rage::scrThreadInfo*>(&m_context));
			m_context.CopyReferencedParametersOut();
		}
		else {
			LOG(Warn, "[base::invoker::end] Failed to find 0x{:X}'s translated hash", hash);
			return;
		}
	}
}