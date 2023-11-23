#include "Hooking/Hooking.hpp"
#include "R.A.G.Engine/Network.hpp"
#include "Util/Util.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::constructAddGamerToSessionCmd(void* _This, rage::snMsgAddGamerToSessionCmd* pGamerCmd) {
		CPlayerInfo* cPlayerInfo{};
		if (auto cPedFactory = *pointers::g_pedFactory; cPedFactory) {
			if (auto cPed = cPedFactory->m_local_ped; cPed) {
				cPlayerInfo = cPed->m_player_info;
			}
		}
		auto rlGamerInfo = cPlayerInfo->m_gamer_info;
		auto& gamerInfo = pGamerCmd->m_gamer_info;
		auto isLocalPlayer = pGamerCmd->m_gamer_info.m_gamer_handle.m_rockstar_id == rlGamerInfo.m_gamer_handle.m_rockstar_id;
		if (isLocalPlayer) {
			//if (auto& data = features::spoofing::name; data.second) {
			//	strcpy(gamerInfo.m_name, data.first.c_str());
			//}
			auto& socketAddr = features::spoofing::socketAddress;
			if (auto& addr = socketAddr.relayAddr; addr.second.first || addr.second.second) {
				auto& addrToSet = gamerInfo.m_relay_address;
				if (auto& data = addr.first.m_ip; addr.second.first) {
					addrToSet.m_ip = data;
				}
				if (auto& data = addr.first.m_port; addr.second.second) {
					addrToSet.m_port = data;
				}
			}
			if (auto& addr = socketAddr.publicAddr; addr.second.first || addr.second.second) {
				auto& addrToSet = gamerInfo.m_public_address;
				if (auto& data = addr.first.m_ip; addr.second.first) {
					addrToSet.m_ip = data;
				}
				if (auto& data = addr.first.m_port; addr.second.second) {
					addrToSet.m_port = data;
				}
			}
			if (auto& addr = socketAddr.localAddr; addr.second.first || addr.second.second) {
				auto& addrToSet = gamerInfo.m_local_address;
				if (auto& data = addr.first.m_ip; addr.second.first) {
					addrToSet.m_ip = data;
				}
				if (auto& data = addr.first.m_port; addr.second.second) {
					addrToSet.m_port = data;
				}
			}
			//if (auto& data = features::spoofing::rid; data.second) {
			//	gamerInfo.m_gamer_handle = data.first;
			//	gamerInfo.m_unk_gamer_handle = data.first;
			//}
		}
		auto res = CALL(constructAddGamerToSessionCmd, _This, pGamerCmd);
		if (features::spoofing::name.second) {
			memcpy(pGamerCmd->m_gamer_info.m_name, rlGamerInfo.m_name, sizeof(pGamerCmd->m_gamer_info.m_name));
		}
		return res;
	}
}