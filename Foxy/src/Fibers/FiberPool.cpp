#include "FiberPool.hpp"
#include "Core/Logger.hpp"

namespace base {
	void fiberPool::create() {
		g_fiberMgr.add(xorstr_("fiberPool"), m_numberOfFibers, &fiberFunction);
	}

	void fiberPool::fiberFunction() {
		while (true) {
			g_fiberPool.tick();
			fiber::cur()->wait();
		}
	}

	void fiberPool::queue(std::function<void()> func) {
		if (func) {
			std::lock_guard lck(m_mutex);
			m_jobs.push(std::move(func));
		}
	}

	void fiberPool::tick() {
		std::unique_lock lck(m_mutex);
		if (!m_jobs.empty()) {
			auto job = m_jobs.top();
			if (job) {
				m_jobs.pop();
				lck.unlock();
				job();
			}
		}
	}
}