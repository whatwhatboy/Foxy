#pragma once
#include "FiberMgr.hpp"

namespace base {
	class dxFiberMgr {
	public:
		void add(char const* fbrId, fnptr<void()> func, std::optional<std::size_t> stackSize = std::nullopt);
		void remove(char const* fbrId);
	public:
		void removeAll();
	public:
		void tick();
	public:
		std::recursive_mutex m_mutex{};
		std::map<char const*, std::unique_ptr<fiber>> m_fibers{};
	};
	inline dxFiberMgr g_dxFiberMgr{};
}