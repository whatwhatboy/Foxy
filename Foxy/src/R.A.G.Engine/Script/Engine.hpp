#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/AtArray.hpp"
#include "R.A.G.Engine/Script/Thread.hpp"

namespace rage {
	template <typename t>
	using pgCollection = atArray<t>;
	template <typename t>
	using pgPtrCollection = atArray<t*>;
	inline std::vector<std::pair<scriptThread*, scriptThread*>> g_ownedThreads;
	namespace scrEngine {
		inline pgPtrCollection<scriptThread>* getThreadCollection() {
			return (pgPtrCollection<scriptThread>*)pointers::g_gtaThreads;
		}
		inline uint32_t& getThreadID() {
			return *pointers::g_threadId;
		}
		inline uint32_t& getThreadCount() {
			return *pointers::g_threadCount;
		}
		inline void createThread(scriptThread* thread) {
			auto& collection{ *getThreadCollection() };
			auto& id{ getThreadID() };
			auto& count{ getThreadCount() };
			int slot{};
			//Get slot
			for (auto& thr : collection) {
				auto& ctx{ thr->m_context };
				if (!ctx.m_thread_id)
					break;
				slot++;
			}
			bool didWeGetSlot{ slot != collection.size() };
			if (didWeGetSlot) {
				std::string name{ std::format("thr_{}", count + 1) };
				auto& ctx{ thread->m_context };
				strcpy(thread->m_name, name.c_str());
				thread->m_script_hash = rage::atStringHash(thread->m_name);
				thread->reset(thread->m_script_hash, nullptr, 0);
				if (!id) {
					id++;
				}
				ctx.m_thread_id = id;
				count++;
				id++;
				auto og{ collection[slot] };
				collection[slot] = thread;
				g_ownedThreads.push_back(std::make_pair(thread, og));
				LOG_DEBUG("Successfully created thread '{}' at id {}", thread->m_name, ctx.m_thread_id);
			}
		}
		inline void cleanupThreads() {
			std::reverse(g_ownedThreads.begin(), g_ownedThreads.end());
			auto& collection{ *getThreadCollection() };
			int slot{};
			for (auto& thr : collection) {
				for (auto& [ownedThr, ogThr] : g_ownedThreads) {
					if (thr == ownedThr) {
						collection[slot] = ogThr;
					}
				}
				slot++;
			}
			g_ownedThreads.clear();
		}
	}
}