#include "Core.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "Hooking/Hooking.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberMgr.hpp"
#include "Fibers/DXFiberMgr.hpp"
#include "Fibers/FiberPool.hpp"
#include "DirectX/Renderer/Renderer.hpp"
#include "Script/Main/MainScript.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "Features/Features.hpp"
#include "Auth/Auth.hpp"
#include "Features/Lua/Lua.hpp"
#include "Hooking/Hooks/Natives/NativeHooking.hpp"
#include "Features/YSC/YSCLoader.hpp"
#include "Exceptions/ExceptionHandler.hpp"
#include "Memory/Util.hpp"
#include "Auth/XorStr.hpp"
#include "Discord/RPC.hpp"
#include "R.A.G.Engine/scrNativeRegistrationTable.hpp"
#include "Memory/ScriptPatcher.hpp"
#include "HTTP/HTTPRequest.hpp"
#include "HTTP/HTTPSRequest.hpp"
#include "ScriptHookV/ScriptHookVLoader.hpp"
#include "Config/Config.hpp"
#include "Config/ScriptHookVConfig.hpp"
#include <regex>

#define SANITY_CHECK_STD_POINTER(v) \
if (!v.get() || !isValidPtr(v.get())) { \
	LOG_DIRECT(eLogType::Fatal, "[SANITY_CHECK_STD_POINTER(base::" STRINGIFY(v) ")]: CHECK FAIL!"); \
	return false; \
} \
else { \
	LOG_DIRECT(eLogType::File, "[SANITY_CHECK_STD_POINTER(base::" STRINGIFY(v) ")]: CHECK OK."); \
}
#define SANITY_CHECK_POINTER(v) \
if (!v || !isValidPtr(v)) { \
	LOG_DIRECT(eLogType::Fatal, "[SANITY_CHECK_POINTER(base::" STRINGIFY(v) ")]: CHECK FAIL!"); \
	return false; \
} \
else { \
	LOG_DIRECT(eLogType::File, "[SANITY_CHECK_STD_POINTER(base::" STRINGIFY(v) ")]: CHECK OK."); \
}
#define SANITY_CHECK_VARIABLE(v) \
if (!v || !isValidPtr(v)) { \
	LOG_DIRECT(eLogType::Fatal, "[SANITY_CHECK_VARIABLE(base::" STRINGIFY(v) ")]: CHECK FAIL!"); \
	return false; \
} \
else { \
	LOG_DIRECT(eLogType::File, "[SANITY_CHECK_VARIABLE(base::" STRINGIFY(v) ")]: CHECK OK."); \
}
#define SANITY_CHECK_ARRAY(v) \
if (v.empty() || !v.size() || !v.data() || !isValidPtr(v.data())) { \
	LOG_DIRECT(eLogType::Fatal, "[SANITY_CHECK_ARRAY(base::" STRINGIFY(v) ")]: CHECK FAIL!"); \
	return false; \
} \
else { \
	LOG_DIRECT(eLogType::File, "[SANITY_CHECK_ARRAY(base::" STRINGIFY(v) ")]: CHECK OK."); \
}
#define SANITY_CHECK_MAP(v) \
if (v.empty() || !v.size()) { \
	LOG_DIRECT(eLogType::Fatal, "[SANITY_CHECK_ARRAY(base::" STRINGIFY(v) ")]: CHECK FAIL!"); \
	return false; \
} \
else { \
	LOG_DIRECT(eLogType::File, "[SANITY_CHECK_ARRAY(base::" STRINGIFY(v) ")]: CHECK OK."); \
}
#define SANITY_CHECK_STACK(v) \
if (!v.size() || !v.top()) { \
	LOG_DIRECT(eLogType::Fatal, "[SANITY_CHECK_ARRAY(base::" STRINGIFY(v) ")]: CHECK FAIL!"); \
	return false; \
} \
else { \
	LOG_DIRECT(eLogType::File, "[SANITY_CHECK_ARRAY(base::" STRINGIFY(v) ")]: CHECK OK."); \
}
#pragma message("--- Build information ---")
#pragma message("-- Build Number: " BUILD_NUMBER " --")
#pragma message("-- Version Number: " VERSION_NUMBER " --")
#pragma message("-- Build Date: " BUILD_DATE " --")

namespace base::sanity {
	bool check() {
		SANITY_CHECK_STD_POINTER(g_logger);
		SANITY_CHECK_STD_POINTER(g_hooking);
		SANITY_CHECK_STD_POINTER(g_renderer);
		SANITY_CHECK_STD_POINTER(g_nativeHooks);
		SANITY_CHECK_VARIABLE(g_module);
		SANITY_CHECK_VARIABLE(security::g_ntdll);
		SANITY_CHECK_VARIABLE(g_auth.m_username);
		SANITY_CHECK_VARIABLE(g_auth.m_password);
		SANITY_CHECK_VARIABLE(pointers::g_handleToPointer);
		SANITY_CHECK_VARIABLE(pointers::g_pointerToHandle);
		SANITY_CHECK_VARIABLE(pointers::g_gtaThreads);
		SANITY_CHECK_VARIABLE(pointers::g_network);
		SANITY_CHECK_VARIABLE(pointers::g_netEventMgr);
		SANITY_CHECK_VARIABLE(pointers::g_scrProgramTbl);
		SANITY_CHECK_VARIABLE(pointers::g_modelTable);
		SANITY_CHECK_VARIABLE(pointers::g_pedFactory);
		SANITY_CHECK_VARIABLE(pointers::g_networkPlayerMgr);
		SANITY_CHECK_VARIABLE(pointers::g_netObjMgr);
		SANITY_CHECK_VARIABLE(pointers::g_friendRegistry);
		SANITY_CHECK_VARIABLE(pointers::g_socialClubInfo);
		SANITY_CHECK_VARIABLE(pointers::g_nativeRegTbl);
		SANITY_CHECK_VARIABLE(pointers::g_jmpRbxRegister);
		SANITY_CHECK_VARIABLE(pointers::g_hwnd);
		SANITY_CHECK_MAP(g_fiberMgr.m_fibers);
		return true;
	}
}
namespace base::core {
	inline HMODULE g_scyllaHideModule{};
	inline stopwatch g_moduleStopwatch{};
	void init() {
		g_auth.init();
		g_running = true;
		stopwatch stopWatch{};
		pointers::scanPointers();
		if (memory::util::g_failedSigCount) {
			g_running = false;
			return;
		}
		pointers::applyPatches();
		auto pointerScanStopwatchValue = stopWatch.getAs<float>();
		g_invoker.cache();
		g_renderer = std::make_unique<renderer>();
		g_scriptPatcher = std::make_unique<scriptPatcher>();
		stopWatch = stopwatch();
		g_hooking = std::make_unique<hooking>();
		auto hookingCreationStopwatchValue = stopWatch.getAs<float>();
		rage::scrEngine::createThread(&g_fiberMgr);
		g_fiberMgr.add(xorstr_("uT"), &util::tick);
		g_fiberMgr.add(xorstr_("fT"), &features::tick);
		g_fiberMgr.add(xorstr_("mST"), &mainScript::tick);
		g_fiberMgr.add(xorstr_("dRPF"), &discordRPC::fiber);
		g_dxFiberMgr.add(xorstr_("dxMT"), [] {
			while (true) {
				g_dxUiManager.handleDrawing();
				g_rendererQueue.tick();
				fiber::cur()->wait();
			}
		});
		g_fiberPool.create();
		stopWatch = stopwatch();
		g_hooking->hook();
		auto hookingEnableStopwatchValue = stopWatch.getAs<float>();
		//g_lua.init();
		g_handshakeHandler.load();
		g_dxUiManager.sendNotify(xorstr_("Welcome"), xorstr_("Welcome aboard, {}"), g_auth.m_username);
		g_dxUiManager.sendNotify(xorstr_("Version"), xorstr_("Build number " BUILD_NUMBER));
		g_dxUiManager.sendNotify(xorstr_("Version"), xorstr_("Version number " VERSION_NUMBER));
		LOG(Info, "Total time to inject the module was {}ms, hooks created in {}ms and pointers found in {}ms", g_moduleStopwatch.getAs<float>(), hookingCreationStopwatchValue + hookingEnableStopwatchValue, pointerScanStopwatchValue);
		if (!sanity::check()) {
			LOG_DIRECT(eLogType::Fatal, "[base::sanity::check]: CHECK FAIL!");
			abort();
		}
		else {
			LOG_DIRECT(eLogType::Info, "[base::sanity::check]: CHECK OK.");
		}
	}
	void uninit() {
		g_running = false;
		//g_handshakeHandler.free();
		g_hooking->unhook();
		if (memory::util::g_failedSigCount) {
			return;
		}
		std::this_thread::sleep_for(1000ms);
		g_hooking.reset();
		g_renderer.reset();
		g_scriptPatcher.reset();
		g_discordRPC.uninit();
		g_fiberMgr.removeAll();
		g_scriptHookV.clear();
		g_dxFiberMgr.removeAll();
		rage::scrEngine::cleanupThreads();
		pointers::restorePatches();
		if (security::g_ntdll)
			FreeLibrary(security::g_ntdll);
		if (g_scyllaHideModule)
			FreeLibrary(g_scyllaHideModule);
		#ifndef DISABLE_SECURITY
			security::restoreHeaders(g_module);
		#endif
		exceptions::uninitExceptionHandler();
		LOG(Info, BRANDING_NAME " has been uninjected.");
	}
	namespace dllMain {
		void dllAttach() {
			g_logger = std::make_unique<logger>();
			exceptions::initExceptionHandler();
			createThreadInstance();
			auto path = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME "\\Modules"));
			if (fs::exists(path))
				g_scyllaHideModule = LoadLibraryA(path.append(xorstr_("ScyllaHide.dll")).string().c_str());
		}
		void dllDetach() {
			core::uninit();
			g_logger.reset();
		}
		void createThreadInstance() {
			CreateThread(nullptr, 0, [](void* ptr) -> DWORD {
				#ifndef DISABLE_SECURITY
				if (auto hmod{ GetModuleHandleA(xorstr_("ScyllaHide.dll")) }) {
					FreeLibrary(hmod);
				}
				if (auto hmod{ GetModuleHandleA(xorstr_("HookLibraryx64.dll")) }) {
					FreeLibrary(hmod);
				}
				security::hideThread(GetCurrentThread());
				#endif
				core::init();
				while (g_running && g_auth.m_isLoggedIn) {
					/*if (!g_handshakeHandler.handshakeSent())
						g_handshakeHandler.handle();*/
					if (GetAsyncKeyState(VK_END))
						g_running = false;
					if (GetAsyncKeyState(VK_F1))
						security::bsod();
					std::this_thread::sleep_for(1ms);
				}
				dllDetach();
				FreeLibraryAndExitThread(g_module, 0);
				return 0;
			}, nullptr, 0, nullptr);
			#ifndef DISABLE_SECURITY
			//TODO: Add HWID + RID to auth
			std::thread sT([&] {
				security::hideThread(GetCurrentThread());
				while (g_running) {
					if (auto hwnd = FindWindowA(nullptr, xorstr_("Select Foxy")); hwnd) {
						g_logger.get()->freeConsoleHandles();
						g_logger.get()->freeFileHandles();
					}
					if (!g_auth.ensureOnlineConnection())
						exit(0);
					if (security::checkDebuggerPresent())
						security::bsod();
					if (!g_auth.m_isLoggedIn || !g_auth.m_username || !g_auth.m_password)
						exit(0);
					std::this_thread::sleep_for(2s);
				}
			});
			sT.detach();
			#endif
		}
	}
}