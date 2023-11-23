#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/ScrThread.hpp"
#include "R.A.G.Engine/SysMemAllocator.hpp"
#include "Memory/Patterns/Pointers.hpp"

namespace base::threading {
	template <typename call, typename ...arguments>
		requires isInvokableWithArgs<call, arguments...>
	bool executeUnderScr(GtaThread* scr, call&& callback, arguments&&... args) {
		auto tlsCtx = rage::tlsContext::get();
		if (!scr || !scr->m_context.m_thread_id)
			return false;
		auto ogThr = tlsCtx->m_script_thread;
		tlsCtx->m_script_thread = scr;
		tlsCtx->m_is_script_thread_active = true;
		std::invoke(std::forward<call>(callback), std::forward<arguments>(args)...);
		tlsCtx->m_script_thread = ogThr;
		tlsCtx->m_is_script_thread_active = ogThr != nullptr;
		return true;
	}
	inline auto getThr = [=](char const* name) -> GtaThread* {
		for (auto& thr : *pointers::g_gtaThreads) {
			if (thr->m_name == name)
				return thr;
		}
		return nullptr;
	};
	inline auto getThrUsingHash = [=](uint32_t hash) -> GtaThread* {
		for (auto& thr : *pointers::g_gtaThreads) {
			if (thr->m_script_hash == hash)
				return thr;
		}
		return nullptr;
	};
}