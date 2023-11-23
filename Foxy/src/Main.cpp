#include "Core/Core.hpp"

using namespace base;
BOOL DllMain(HINSTANCE hInstance, DWORD reason, LPVOID) {
	g_module = hInstance;
	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		core::dllMain::dllAttach();
	} break;
	case DLL_PROCESS_DETACH: {
		core::dllMain::dllDetach();
	} break;
	}
	return TRUE;
}