#include "Hooking/Hooking.hpp"
#include "Hooking/Hooks/Natives/NativeHooking.hpp"
#include "Memory/ScriptPatcher.hpp"
#include "R.A.G.Engine/scrOpcode.hpp"
#include "R.A.G.Engine/scrValue.hpp"
#include "R.A.G.Engine/ScrProgram.hpp"

const u32 scrPageShift = 14;
const u32 scrPageSize = (1 << scrPageShift);
const u32 scrPageMask = scrPageSize - 1;
#define CASE(x) case x:
#define DEFAULT default:
#define FETCH_INSN
#define NEXT_INSN goto DISPATCH

#define VALIDATE_PC(target) do { if (target < 4) FAULT("Impossible jump target caught"); } while (0)
#define VALIDATE_PTR(value)	do { if (value.Uns < 16384) FAULT("Invalid reference caught"); } while (0)

#define SET_PC(_o) do { u32 o = _o; pc = (opcodesTbl[o>>scrPageShift] + (o & scrPageMask) - 1); opcodes = pc - o; } while (0)
#define ADD_PC(_r) SET_PC((pc - opcodes) + (_r))
#define CHECK_PC SET_PC(pc - opcodes)
#define DEC_CALLDEPTH --(ser->m_call_depth)
#define SET_STATE_AND_RET(state) ser->m_state = state; return ser->m_state;

#define FAULT(s, ...) do { LOG(Fatal, s, __VA_ARGS__); NEXT_INSN; } while(0)

#define LoadImm32	((pc+=4), *(u32*)(pc-3))
#if defined(__SNC__) || defined(_XBOX_VER)
#define LoadImm24	((pc+=3), *(u32*)(pc-3) & 0xFFFFFF)
#else
#define LoadImm24	((pc+=3), *(u32*)(pc-3) >> 8)
#endif
#define LoadImm16	((pc+=2), *(u16*)(pc-1))
#define LoadImmS16	((pc+=2), *(s16*)(pc-1))
#define LoadImm8	(*++pc)

static void scr_assign_string(char* dst, unsigned siz, const char* src)
{
	if (src)
	{
		while (*src && --siz)
			*dst++ = *src++;
	}
	*dst = '\0';
}

static void scr_append_string(char* dst, unsigned siz, const char* src)
{
	while (*dst)
		dst++, --siz;
	scr_assign_string(dst, siz, src);
}

static void scr_itoa(char* dest, int value)
{
	char stack[16], * sp = stack;
	if (value < 0)
	{
		*dest++ = '-';
		value = -value;
	}
	else if (!value)
	{
		dest[0] = '0';
		dest[1] = 0;
		return;
	}
	while (value)
	{
		*sp++ = (char)((value % 10) + '0');
		value /= 10;
	}
	while (sp != stack)
	{
		*dest++ = *--sp;
	}
	*dest = 0;
}

inline float scr_fmodf(float x, float y) { return y ? x - ((int)(x / y) * y) : 0; }
enum { c_NativeInsnLength = 4 };
static const int MAX_CALLSTACK = 16;
namespace base {
	rage::eThreadState hooks::scriptVirtualMachine(rage::scrValue* stack, rage::scrValue* globals, rage::scrProgram* pt, rage::scrThreadContext* ser) {
		RET_CALL(scriptVirtualMachine, stack, globals, pt, ser)
	}
}
