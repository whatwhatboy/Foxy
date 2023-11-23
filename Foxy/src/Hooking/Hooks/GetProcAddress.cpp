#include "Hooking/Hooking.hpp"
#include "ScriptHookV/ScriptHookVLoader.hpp"
#include "ScriptHookV/ScriptHookV.hpp"

namespace base {
	FARPROC hooks::getProcAddress(HMODULE hModule, LPCSTR lpProcName) {
		if (hModule == g_handshakeHandler.getExportModule()) {
			switch (rage::atStringHash(lpProcName)) {
			case "createTexture"_joaat: { return FARPROC(&shv::createTexture); } break;
			case "drawTexture"_joaat: { return FARPROC(&shv::drawTexture); } break;
			case "getGameVersion"_joaat: { return FARPROC(&shv::getGameVersion); } break;
			case "getGlobalPtr"_joaat: { return FARPROC(&shv::getGlobalPtr); } break;
			case "getScriptHandleBaseAddress"_joaat: { return FARPROC(&shv::getScriptHandleBaseAddress); } break;
			case "keyboardHandlerRegister"_joaat: { return FARPROC(&shv::keyboardHandlerRegister); } break;
			case "keyboardHandlerUnregister"_joaat: { return FARPROC(&shv::keyboardHandlerUnregister); } break;
			case "nativeCall"_joaat: { return FARPROC(&shv::nativeCall); } break;
			case "nativeInit"_joaat: { return FARPROC(&shv::nativeInit); } break;
			case "nativePush64"_joaat: { return FARPROC(&shv::nativePush64); } break;
			case "presentCallbackRegister"_joaat: { return FARPROC(&shv::presentCallbackRegister); } break;
			case "presentCallbackUnregister"_joaat: { return FARPROC(&shv::presentCallbackUnregister); } break;
			case "scriptRegister"_joaat: { return FARPROC(&shv::scriptRegister); } break;
			case "scriptRegisterAdditionalThread"_joaat: { return FARPROC(&shv::scriptRegisterAdditionalThread); } break;
			case "scriptUnregister"_joaat: { return FARPROC(&shv::scriptUnregister); } break;
			case "scriptUnregisterDepricated"_joaat: { return FARPROC(&shv::scriptUnregisterDepricated); } break;
			case "scriptWait"_joaat: { return FARPROC(&shv::scriptWait); } break;
			case "worldGetAllObjects"_joaat: { return FARPROC(&shv::worldGetAllObjects); } break;
			case "worldGetAllPeds"_joaat: { return FARPROC(&shv::worldGetAllPeds); } break;
			case "worldGetAllPickups"_joaat: { return FARPROC(&shv::worldGetAllPickups); } break;
			case "worldGetAllVehicles"_joaat: { return FARPROC(&shv::worldGetAllVehicles); } break;
			}
		}
		RET_CALL_DECL(getProcAddress, hModule, lpProcName);
	}
}