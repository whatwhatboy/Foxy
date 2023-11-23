#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"
#include "Features/Features.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "R.A.G.Engine/scrNativeCallContext.hpp"
#include "Util/Util.hpp"
#define DEFINE_SCRIPT_HASH_THIS_FRAME() Hash ScriptHashThisFrame = SCRIPT::GET_HASH_OF_THIS_SCRIPT_NAME();
#define IS_VIABLE_SCRIPT(hash) hash == "main"_joaat || hash == "freemode"_joaat || hash == "main_persistent"_joaat
#define ENSURE_VIABLE_SCRIPT_THIS_FRAME() DEFINE_SCRIPT_HASH_THIS_FRAME() \
	if (IS_VIABLE_SCRIPT(ScriptHashThisFrame)) { \
	} \
	else { \
		return; \
	}
#define FRAME_COUNT(value) static uint32_t FrameCount = value;
#define BEGIN_MATCHING_FRAME_ENSURE() if (FrameCount != MISC::GET_FRAME_COUNT()) {
#define END_MATCHING_FRAME_ENSURE() FrameCount = MISC::GET_FRAME_COUNT(); \
	}

namespace base::natives {
	namespace PLAYER_D {
		inline void GET_PLAYER_NAME(class rage::scrThreadInfo* scr) {
			const auto playerId = scr->Params[0].Int;
			if (features::spoofing::name.second && playerId == util::player::g_entityHandle) {
				scr->ResultPtr->String = features::spoofing::name.first.c_str();
			}
			else {
				scr->ResultPtr->String = PLAYER::GET_PLAYER_NAME(playerId);
			}
		}
	}
	namespace NETWORK_D {
		inline void NETWORK_BAIL(class rage::scrThreadInfo* scr) {
			g_dxUiManager.sendNotify(xorstr_("Network"), xorstr_("Bail ({}, {}, {}) prevented"), scr->Params[0].Int, scr->Params[1].Int, scr->Params[2].Int);
		}
		inline void NETWORK_CAN_BAIL(class rage::scrThreadInfo* scr) {
			g_dxUiManager.sendNotify(xorstr_("Network"), xorstr_("Bail check prevented"));
			scr->ResultPtr->Int = FALSE;
		}
		inline void NETWORK_SESSION_HOST(class rage::scrThreadInfo* scr) {
			if (features::m_data.isQueued) {
				pointers::g_joinBySessionInfo(util::network::get(), &features::m_data.sessionInfo, 1, 1 | 2 | 4, nullptr, 0);
				features::m_data.isQueued = false;
				scr->ResultPtr->Int = TRUE;
			}
			else
				scr->ResultPtr->Int = NETWORK::NETWORK_SESSION_HOST(scr->Params[0].Int, scr->Params[1].Int, scr->Params[2].Int);
		}
	}
	namespace DLC_D {
		inline void IS_DLC_PRESENT(class rage::scrThreadInfo* scr) {
			const auto hash = scr->Params[0].Uns;
			if (hash == 0x96F02EE6 && features::loadDevDLC)
				scr->ResultPtr->Int = TRUE;
			else
				scr->ResultPtr->Int = DLC::IS_DLC_PRESENT(hash);
		}
	}
	namespace CAM_D {
		inline void DO_SCREEN_FADE_IN(class rage::scrThreadInfo* scr) {
			if (features::instantRespawnBool)
				return;
			const auto duration = scr->Params[0].Int;
			if (!PED::IS_PED_DEAD_OR_DYING(PLAYER::PLAYER_PED_ID(), TRUE))
				CAM::DO_SCREEN_FADE_IN(duration);
		}
		inline void DO_SCREEN_FADE_OUT(class rage::scrThreadInfo* scr) {
			if (features::instantRespawnBool)
				return;
			const auto duration = scr->Params[0].Int;
			if (!PED::IS_PED_DEAD_OR_DYING(PLAYER::PLAYER_PED_ID(), TRUE))
				CAM::DO_SCREEN_FADE_OUT(duration);
		}
	}
}