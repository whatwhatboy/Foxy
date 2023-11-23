#pragma once
#include "Pch/Common.hpp"
#include "TlsContext.hpp"
#include "Joaat.hpp"
#include "scrValue.hpp"

class CGameScriptHandlerNetComponent;
namespace rage {
	//Thread State
	enum class eThreadState {
		running,
		blocked,
		aborted,
		halted,
		reset_instruction_count
	};
	//Thread
#pragma pack(push, 8)
	class scrThreadContext {
	public:
		uint32_t m_thread_id; //0x0000
		joaat_t m_script_hash; //0x0004
		eThreadState m_state; //0x0008
		uint32_t m_pointer_count; //0x000C
		uint32_t m_frame_pointer; //0x0010
		uint32_t m_stack_pointer; //0x0014
		float m_timer_a; //0x0018
		float m_timer_b; //0x001C
		float m_wait; //0x0020
		int32_t m_min_pc; //0x0024
		int32_t m_max_pc; //0x0028
		char m_tls[36]; //0x002C
		uint32_t m_stack_size; //0x0050
		uint32_t m_catch_pointer_count; //0x0054
		uint32_t m_catch_frame_pointer; //0x0058
		uint32_t m_catch_stack_pointer; //0x005C
		uint32_t m_priority; //0x0060
		uint8_t m_call_depth; //0x0060
		uint8_t unk_0061; //0x0061
		uint16_t unk_0062; //0x0062
		char m_callstack[16]; //0x0068
	}; //Size: 0x0078
	static_assert(sizeof(scrThreadContext) == 0x78);
#pragma pack(pop)
	class scrThread {
	public:
		virtual ~scrThread() = default;                   //0 (0x00)
		virtual eThreadState reset(uint32_t script_hash, void* args, uint32_t arg_count) { return m_context.m_state; }     //1 (0x08)
		virtual eThreadState run() { return m_context.m_state; }                //2 (0x10)
		virtual eThreadState tick(uint32_t ops_to_execute) { return m_context.m_state; }          //3 (0x18)
		virtual void kill() {}                //4 (0x20)
		static scrThread** getPointer() {
			auto tls = uint64_t(rage::tlsContext::get());
			return reinterpret_cast<scrThread**>(tls + offsetof(rage::tlsContext, m_script_thread));
		}
		static scrThread* get() {
			return rage::tlsContext::get()->m_script_thread;
		}
	public:
		scrThreadContext* getContext() {
			return &m_context;
		}
		void setScriptName(const char* name) {
			strncpy(m_name, name, sizeof(m_name) - 1);
			m_name[sizeof(m_name) - 1] = '\0';
		}
	public:
		class scrThreadContext m_context; //0x0000
		char unk_0078[48]; //0x0078
		scrValue* m_stack; //0x00B0
		uint32_t unk_00B8; //0x00B8
		uint32_t m_arg_size; //0x00BC
		uint32_t m_arg_loc; //0x00C0
		uint32_t unk_00C4; //0x00C4 - Some sort of arg
		const char* m_exit_message; //0x00C8
		uint32_t unk_00D0; //0x00D0
		char m_name[64]; //0x00D4
		class scriptHandler* m_handler; //0x0110
		class CGameScriptHandlerNetComponent* m_net_component; //0x0118
	}; //Size: 0x0128
	static_assert(sizeof(scrThread) == 0x128);
}

class GtaThread : public rage::scrThread {
public:
	uint32_t m_script_hash; //0x0128
	uint32_t m_pointer_count_cleanup; //0x012C
	uint32_t m_unk_130; //0x0130
	uint32_t m_unk_134; //0x0134
	uint32_t m_unk_138; //0x0138
	int32_t m_unk_13C; //0x013C
	int32_t m_instance_id; //0x0140
	uint32_t m_base_instance_id; //0x0144
	uint8_t m_flag; //0x0148
	bool m_safe_for_network_game; //0x0149
	bool m_allow_host_broadcast; //0x014A
	bool m_unk_14B; //0x014A
	bool m_is_minigame_script; //0x014C
	char pad_014D[2]; //0x014D
	bool m_can_be_paused; //0x014F
	bool m_can_remove_blips_from_other_scripts; //0x0150
	bool unk_0151; //0x0151
	bool m_allow_attachment; //0x0152
	bool m_detached; //0x0153
	uint8_t m_flag_2; //0x0154
	char pad_0155[11]; //0x0155
};
static_assert(sizeof(GtaThread) == 0x160);