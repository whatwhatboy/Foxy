#include "Pch/Common.hpp"
#include "Core/Core.hpp"
#include "Config/Config.hpp"
#include "Exports/Exports.hpp"

namespace Core {
	void Initialize() {
		if (!AttachConsole(GetCurrentProcessId()))
			AllocConsole();
		SetConsoleTitleA("ScriptHookV");
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
		SetConsoleCP(CP_UTF8);
		SetConsoleOutputCP(CP_UTF8);
		g_Config.Load();
		printf("Config loaded\n");
		if (!g_Config.Get()["HandshakeSent"].get<bool>()) {
			g_Config.Get()["ModuleAddress"] = (uint64_t)GetModuleHandleA("ScriptHookV.dll");
			printf("Module address = %llx\n", (uint64_t)GetModuleHandleA("ScriptHookV.dll"));
			g_Config.Get()["HandshakeSent"] = true;
			printf("Handshake sent\n");
		}
		g_Config.Save();
		Global::g_ExportModuleAddress = NULL;
	}
	void Loop() {
		while (g_Config.Get()["Running"].get<bool>()) {
			if (GetAsyncKeyState(VK_DELETE)) {
				g_Config.Get()["Running"] = false;
				g_Config.Save();
			}
			if (Global::g_ExportModuleAddress == 0 && g_Config.Get()["HandshakeReceived"].get<bool>()) {
				printf("Handshake received\n");
				Global::g_ExportModuleAddress = g_Config.Get()["ExportModuleAddress"].get<uint64_t>();
				printf("Export module = %llx\n", Global::g_ExportModuleAddress);
			}
			else if (!Global::g_ExportModuleAddress) {
				printf("Module null but no handshake\n");
			}
			Global::g_ExportModule = (HMODULE)Global::g_ExportModuleAddress;
		}
	}
	void Uninitialize(HMODULE HMod) {
		printf("Uninit called\n");
		FreeLibraryAndExitThread(HMod, NULL);
	}
}
namespace Thread {
	DWORD Entry(LPVOID Param) {
		Core::Initialize();
		Core::Loop();
		Core::Uninitialize((HMODULE)Param);
		return 0;
	}
	BOOL Create(HMODULE HMod, DWORD CallReason) {
		switch (CallReason) {
		case DLL_PROCESS_ATTACH:
			Global::g_Running = true;
			Global::g_Thread = CreateThread(nullptr, NULL, Entry, HMod, NULL, nullptr);
			break;
		case DLL_PROCESS_DETACH:
			Global::g_Running = false;
			CloseHandle(Global::g_Thread);
			break;
		}
		return TRUE;
	}
}