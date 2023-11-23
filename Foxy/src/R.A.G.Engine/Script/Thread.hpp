#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/ScrThread.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"
#include "Memory/Util.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "R.A.G.Engine/ScrHandlers.hpp"
using namespace base;

inline uint32_t g_frameCount{};
namespace rage {
	class scriptThread : public GtaThread {
	public:
		void init() {
			return pointers::g_scrThreadInit(this);
		}
	public:
		virtual void do_run() = 0; //Our run shimmy
		virtual eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count) {
			RtlZeroMemory(&m_context, sizeof(m_context));
			m_context.m_state = eThreadState::blocked;
			m_context.m_script_hash = script_hash;
			m_context.m_min_pc = -1;
			m_context.m_max_pc = -1;
			m_context.m_priority = 1;
			init();
			m_exit_message = "Normal exit";
			return m_context.m_state;
		}
		virtual eThreadState run() {
			auto tls{ tlsContext::get() };
			if (!m_handler) {
				pointers::g_gameScriptHandlerMgr->attach_thread(this);
				m_safe_for_network_game = true;
			}
			auto ogThr = tls->m_script_thread;
			tls->m_script_thread = this;
			if (m_context.m_state != eThreadState::aborted) {
				if (g_frameCount != MISC::GET_FRAME_COUNT()) {
					do_run();
					g_frameCount = MISC::GET_FRAME_COUNT();
				}
			}
			tls->m_script_thread = ogThr;
			return m_context.m_state;
		}
		virtual eThreadState tick(uint32_t ops_to_execute) {
			return pointers::g_scrThreadTick(this, ops_to_execute);
		}
		virtual void kill() {
			return pointers::g_scrThreadKill(this);
		}
	};
}