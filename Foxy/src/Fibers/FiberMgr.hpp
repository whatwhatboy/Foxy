#pragma once
#include "Pch/Common.hpp"
#include "Fiber.hpp"
#include "R.A.G.Engine/Script/Engine.hpp"

namespace base {
	class fiberMgr : public rage::scriptThread {
	public:
		void add(char const* id, fnptr<void()> func, bool antiLog = false, std::optional<std::size_t> stackSize = std::nullopt);
		void add(char const* baseId, int numberOfFibers, fnptr<void()> func, std::optional<std::size_t> stackSize = std::nullopt);
		void remove(char const* id);
		void removeBase(char const* baseId);
	public:
		bool doesFiberExist(char const* fbrId) {
			if (auto val = m_fibers.find(fbrId); val == m_fibers.end())
				return false;
			return true;
		}
	public:
		void removeAll();
	public:
		void do_run() override;
		rage::eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count) override;
	public:
		std::recursive_mutex m_mutex{};
		std::map<char const*, std::unique_ptr<fiber>> m_fibers{};
	};
	inline fiberMgr g_fiberMgr{};
}