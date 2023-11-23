#include "FiberMgr.hpp"

namespace base {
	void fiberMgr::add(char const* id, fnptr<void()> func, bool antiLog, std::optional<std::size_t> stackSize) {
		std::lock_guard lck(m_mutex);
		m_fibers.insert({ id, std::make_unique<fiber>(id, func, stackSize) });
		if (!antiLog)
			LOG(Info, "Created fiber {}", id);
	}
	void fiberMgr::add(char const* baseId, int numberOfFibers, fnptr<void()> func, std::optional<std::size_t> stackSize) {
		for (int32_t i{ numberOfFibers }; i; --i) {
			add(std::format("{}_{}", baseId, i).c_str(), func, true, stackSize);
		}
		LOG(Info, "Created fiber group '{}' with {} fibers", baseId, numberOfFibers);
	}
	void fiberMgr::remove(char const* id) {
		std::lock_guard lck(m_mutex);
		m_fibers.erase(id);
	}
	void fiberMgr::removeBase(char const* baseId) {
		std::lock_guard lck(m_mutex);
		for (auto& f : m_fibers) {
			auto fbrId{ f.first };
			if (strstr(fbrId, baseId)) {
				m_fibers.erase(fbrId);
				return;
			}
		}
	}

	void fiberMgr::removeAll() {
		std::lock_guard lck(m_mutex);
		m_fibers.clear();
	}

	void fiberMgr::do_run() {
		static bool ensureMainFiber = (ConvertThreadToFiber(nullptr), true);
		if (!ensureMainFiber)
			return;
		std::lock_guard lck(m_mutex);
		for (auto& scr : m_fibers) {
			scr.second->tick();
		}
	}
	rage::eThreadState fiberMgr::reset(uint32_t script_hash, void* args, uint32_t arg_count) {
		removeAll();
		return rage::scriptThread::reset(script_hash, args, arg_count);
	}
}