#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"
#include "R.A.G.Engine/CJoinRequestContext.hpp"
#include "R.A.G.Engine/CMsgJoinResponse.hpp"

namespace base {
	constexpr uint32_t statusCodes[23] = {
		5, //0
		1, //1
		6, //2
		7, //3
		8, //4
		9, //5
		10, //6
		12, //7
		13, //8
		14, //9
		16, //10
		17, //11
		18, //12
		19, //13
		20, //14
		21, //15
		22, //16
		23, //17
		25, //18
		26, //19
		27, //20
		28, //21
		29, //22
	};
	bool hooks::handleJoinRequest(Network* _This, rage::snSession* session, rage::rlGamerInfo* gamerInfo, CJoinRequestContext* ctx, BOOL isTransition) {
		for (auto& val : protections::g_arrayOfBlacklistedPlayers) {
			if (val.m_platformData == gamerInfo->m_platform_data) {
				if (auto togType = protections::g_playerBlacklist.toggleType; protections::g_playerBlacklist.toggle) {
					if (togType == eToggleTypes::tog_notify || togType == eToggleTypes::tog_notifyAndBlock)
						g_dxUiManager.sendNotify(xorstr_("Blacklist"), xorstr_("Join from {} ({})"), gamerInfo->m_name, val.m_blacklistReason);
					if (togType == eToggleTypes::tog_block || togType == eToggleTypes::tog_notifyAndBlock) {
						CMsgJoinResponse response{};
						if (std::string(val.m_blacklistReason).find(xorstr_("Spoofing As Friend")) != std::string::npos)
							response.m_status_code = statusCodes[19];
						else if (std::string(val.m_blacklistReason).find(xorstr_("Invite-Only")) != std::string::npos || std::string(val.m_blacklistReason).find(xorstr_("Solo")) != std::string::npos)
							response.m_status_code = statusCodes[16];
						else if (std::string(val.m_blacklistReason).find(xorstr_("Crew")) != std::string::npos)
							response.m_status_code = statusCodes[11];
						else
							response.m_status_code = statusCodes[rng(0, sizeof(statusCodes)).get<uint8_t>()];
						pointers::g_writeJoinResponseData(&response, ctx->m_join_response_data, 512, &ctx->m_join_response_size);
						return false;
					}
				}
			}
		}
		RET_CALL(handleJoinRequest, _This, session, gamerInfo, ctx, isTransition);
	}
}