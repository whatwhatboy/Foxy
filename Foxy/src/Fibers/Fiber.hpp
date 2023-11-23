#pragma once
#include "Pch/Common.hpp"

namespace base {
	class fiber {
	public:
		fiber(char const* name, fnptr<void()> func, std::optional<std::size_t> statckSize);
		~fiber();
	public:
		void tick();
		void wait(std::optional<std::chrono::high_resolution_clock::duration> time = std::nullopt);
		static fiber* cur();
	public:
		void fiberFunc();
	public:
		char const* m_name{};
		void* m_primaryFiber{};
		void* m_secondaryFiber{};
		fnptr<void()> m_func{};
	public:
		std::optional<std::chrono::high_resolution_clock::time_point> m_time{};
	};
}