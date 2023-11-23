#include "Hooking/Hooking.hpp"
#include "Hooking/Hooks/Natives/NativeHooking.hpp"
#include "Features/Features.hpp"
#include "Util/Util.hpp"
#include "Memory/ScriptPatcher.hpp"

namespace base {
	void hooks::networkPlayerMgrShutdown(CNetworkPlayerMgr* _This) {
		g_nativeHooks->searchForValidScriptsAndHook();
		g_dxUiManager.sendNotify(xorstr_("Session"), xorstr_("Left an session with {} players"), _This->m_player_count);
		RET_CALL(networkPlayerMgrShutdown, _This)
	}
}