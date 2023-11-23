#include "Pch/Common.hpp"
#include "Exceptions/ExceptionHandler.hpp"
#include "Stack/Disassmbler.hpp"
#include <Zydis/Zydis.h>
#include "HDE/hde64.h"

namespace base::exceptions {
	uint8_t getInstructionLength(uint8_t* code) {
		hde64s hde{};
		//Most light to most heavy on CPU cycles. It's pretty much a system of what can handle what at what speed
		/*if (uint8_t len = hde64_disasm((void*)code, &hde); len) {
			return len;
		}
		else*/ if (uint8_t len = x64::getInstructionLength(code); len) {
			return len;
		}
		else {
			ZyanU64 opcodeAddress{ (uint64_t)code };
			ZydisDisassembledInstruction instruction{};
			if (ZYAN_SUCCESS(ZydisDisassembleIntel(ZYDIS_MACHINE_MODE_LONG_64, opcodeAddress, reinterpret_cast<void*>(opcodeAddress), 32, &instruction))) {
				return instruction.info.length;
			}
		}
		return NULL;
	}
	bool attemptStackRecovery(PEXCEPTION_POINTERS exceptionInfo) {
		auto& ctx = exceptionInfo->ContextRecord;
		if (const auto len = getInstructionLength(reinterpret_cast<uint8_t*>(ctx->Rip)); len) {
			ctx->Rip += len;
			return true;
		}
		return false;
	}
	std::string getExceptionType(ULONG_PTR type) {
		if (g_exceptionErrorReasons[type]()) {
			return g_exceptionErrorReasons[type].what();
		}
		return "unknown";
	}
	LONG CALLBACK onExceptionCallback(PEXCEPTION_POINTERS exceptionInfo) {
		stackWalker sw(StackWalker::AfterExcept);
		exceptionContext ctx{ exceptionInfo };
		if (!ctx.m_fileoffset.empty()) {
			switch (ctx.m_code) {
			case CONTROL_C_EXIT: { return EXCEPTION_CONTINUE_EXECUTION; } break;
			case EXCEPTION_BREAKPOINT: { return EXCEPTION_CONTINUE_EXECUTION; } break;
			case EXCEPTION_ACCESS_VIOLATION: {
				auto exceptionTypeStr = getExceptionType(ctx.m_type);
				auto exceptionName = g_exceptionErrorReasons[ctx.m_code]() ? g_exceptionErrorReasons[ctx.m_code].what() : std::format("0x{:X}", ctx.m_code);
				LOG(Exception, "The game suffered an fatal exception, you may need to restart the game. ({} at {}, reason of {} was {}{})", exceptionName, ctx.m_fileoffset, exceptionName, exceptionTypeStr, ctx.m_type != 8 && exceptionName != "unknown" ? "" : std::format("0x{:X}", ctx.m_deathAddress));
			} break;
			default: {
				auto exceptionName = g_exceptionErrorReasons[ctx.m_code]() ? g_exceptionErrorReasons[ctx.m_code].what() : std::format("0x{:X}", ctx.m_code);
				LOG(Exception, "The game suffered a exception of unknown severity, you may need to restart the game. ({} at {}, reason of exception is unknown)", exceptionName, ctx.m_fileoffset);
			}
			}
		}
		LOG(Exception, "Dumping registers...");
		ctx.printRegisters();
		//We want use our stripped ver as GetCurrentThread will be nullptr (hide thread go brrrrrrt)
		#ifdef DEBUG_MODE
			LOG(Exception, "Showing callstack...");
			sw.ShowCallstack(GetCurrentThread(), &ctx.m_context);
		#else
			LOG(Exception, "Showing callstack...");
			LOG_UNENCRYPTED(Backtrace, util::callstack::dump());
		#endif
		if (attemptStackRecovery(exceptionInfo)) {
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		else {
			LOG(Exception, "Failed stack recovery, re-attempting exception search for an valid point in the stack...");
		}
		return EXCEPTION_CONTINUE_SEARCH;
	}
	bool onExceptionCallback_TryExcept(PEXCEPTION_POINTERS exceptionInfo) {
		if (onExceptionCallback(exceptionInfo) != EXCEPTION_CONTINUE_EXECUTION)
			return false;
		return true;
	}
	LONG CALLBACK exceptionHandler(PEXCEPTION_POINTERS exceptionInfo) {
		return onExceptionCallback(exceptionInfo);
	}
	LONG CALLBACK unhandledExceptionHandler(PEXCEPTION_POINTERS exceptionInfo) {
		return onExceptionCallback(exceptionInfo);
	}
	void initExceptionHandler() {
	#ifndef DISABLE_EXCEPTION_HANDLER
		vecExcepHandlerHandle = AddVectoredExceptionHandler(FALSE, PVECTORED_EXCEPTION_HANDLER(exceptionHandler));
		SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER(unhandledExceptionHandler));
	#endif
	}
	void uninitExceptionHandler() {
		if (vecExcepHandlerHandle)
			RemoveVectoredExceptionHandler(vecExcepHandlerHandle);
	}
}