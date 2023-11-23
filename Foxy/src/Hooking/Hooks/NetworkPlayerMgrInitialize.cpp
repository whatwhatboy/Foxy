#include "Hooking/Hooking.hpp"
#include "Hooking/Hooks/Natives/NativeHooking.hpp"
#include "Features/Features.hpp"
#include "Util/Util.hpp"

namespace base {
	void hooks::networkPlayerMgrInitialize(CNetworkPlayerMgr* _This, uint64_t ui64Unk, uint32_t ui32Unk, uint32_t ui32Arr4_Unk[4]) {
		g_nativeHooks->searchForValidScriptsAndHook();
		//g_dxUiManager.sendNotify(xorstr_("Session"), xorstr_("Joined an session with {} players ({} of players, hosted by {})"), _This->m_player_count ? std::to_string(_This->m_player_count) : "unknown ammount", _This->m_player_count == _This->m_player_limit ? "Full" : "Not full", _This->GetHost()->GetName());
		RET_CALL(networkPlayerMgrInitialize, _This, ui64Unk, ui32Unk, ui32Arr4_Unk);
	}
}