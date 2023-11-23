#include "Fiber.hpp"
#include "Core/Logger.hpp"

namespace base {
	fiber::fiber(char const* name, fnptr<void()> func, std::optional<std::size_t> statckSize) : m_name(name), m_func(func), m_secondaryFiber(nullptr), m_primaryFiber(nullptr) {
		m_secondaryFiber = CreateFiber(statckSize.has_value() ? statckSize.value() : 0, [](void* param) {
			static_cast<fiber*>(param)->fiberFunc();
		}, this);
	}

	fiber::~fiber() {
		if (m_secondaryFiber)
			DeleteFiber(m_secondaryFiber);
	}

	void fiber::tick() {
		m_primaryFiber = GetCurrentFiber();
		if (!m_time.has_value() || m_time.value() <= std::chrono::high_resolution_clock::now())
			SwitchToFiber(m_secondaryFiber);
	}

	void fiber::wait(std::optional<std::chrono::high_resolution_clock::duration> time) {
		if (time.has_value() && time.value() != 0ms)
			m_time = std::chrono::high_resolution_clock::now() + time.value();
		else
			m_time = std::nullopt;
		SwitchToFiber(m_primaryFiber);
	}

	fiber* fiber::cur() {
		return static_cast<fiber*>(GetFiberData());
	}

	void fiber::fiberFunc() {
		try {
			m_func();
		}
		catch (std::exception& except) {
			LOG(Exception, "Caught exception {} in fiber {}", except.what(), m_name);
		}
		while (true) {
			wait();
		}
	}
}