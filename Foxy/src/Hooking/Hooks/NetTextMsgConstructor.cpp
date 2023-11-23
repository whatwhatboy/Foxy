#include "Hooking/Hooking.hpp"
#include "R.A.G.Engine/CMsgTextMessage.hpp"
#include "Util/Util.hpp"
#include "Util/Chat.hpp"

namespace base {
	bool hooks::netTextMsgConstructor(rage::netConnectionManager* mgr, int msgId, CMsgTextMessage* msg, int flags, void* unk) {
		if (features::spoofing::peerId.second) {
			msg->m_peer_id = features::spoofing::peerId.first;
		}
		auto snPlayer{ util::network::session::player::getViaMsgId(msgId) };
		auto receiver{ util::network::getCNetGamePlayerViaPeerAddress(snPlayer->m_gamer_info.m_peer_address) };
		if (receiver && util::player::g_networkHandle) {
			if (receiver->m_player_id == 0) {
				std::string message(msg->m_message);
				auto words = getWords(message);
				if (message.find(std::format("{} said", util::player::g_networkHandle->GetName())) == -1)
					handleCmds(util::player::g_entityHandle, words);
				g_dxUiManager.sendNotify(xorstr_("Chat"), xorstr_("{} [{}]: {}"), util::player::g_networkHandle->GetName(), util::localization::getGxtLabel(std::format("MP_CHAT_{}", msg->m_is_team ? xorstr_("TEAM") : xorstr_("ALL"))), message);
			}
		}
		if (!features::msgSyncWithPerson[receiver->m_player_id]) {
			memset(msg->m_message, NULL, sizeof(msg->m_message));
			return false;
		}
		RET_CALL(netTextMsgConstructor, mgr, msgId, msg, flags, unk)
	}
}