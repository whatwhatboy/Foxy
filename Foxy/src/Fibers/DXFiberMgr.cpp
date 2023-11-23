#include "DXFiberMgr.hpp"
#include "Threading.hpp"

namespace base {
	void dxFiberMgr::add(char const* fbrId, fnptr<void()> func, std::optional<std::size_t> stackSize) {
		std::lock_guard lck(m_mutex);
		m_fibers.insert({ fbrId, std::make_unique<fiber>(fbrId, func, stackSize) });
	}
	void dxFiberMgr::remove(char const* fbrId) {
		std::lock_guard lck(m_mutex);
		m_fibers.erase(fbrId);
	}

	void dxFiberMgr::removeAll() {
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}

	void dxFiberMgr::tick() {
		static bool ensureMainFiber = (ConvertThreadToFiber(nullptr), true);
		if (!ensureMainFiber)
			return;
		std::lock_guard lck(m_mutex);
		for (auto& scr : m_fibers)
			scr.second->tick();
	}
}