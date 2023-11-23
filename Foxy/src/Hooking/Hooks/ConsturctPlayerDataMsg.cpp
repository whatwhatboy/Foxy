#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::constructPlayerDataMsg(CNetGamePlayerDataMsg* msg, rage::datBitBuffer* buffer) {
		auto ogGroup = msg->m_matchmaking_group;
		if (features::joinInsideSCTVSlots) {
			msg->m_matchmaking_group = 4;
		}
		auto ret = CALL(constructPlayerDataMsg, msg, buffer);
		if (msg->m_matchmaking_group != ogGroup)
			msg->m_matchmaking_group = ogGroup;
		return ret;
	}
}