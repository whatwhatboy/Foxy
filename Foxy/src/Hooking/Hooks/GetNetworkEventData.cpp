#include "Hooking/Hooking.hpp"
#include "R.A.G.Engine/CEventNetwork.hpp"
#include "R.A.G.Engine/Enums.hpp"
#include "Util/Util.hpp"

namespace base {
	void hooks::getNetworkEventData(uint64_t _This, rage::CEventNetwork* netEvent) {
		switch (netEvent->get_type()) {
		case rage::eEventNetworkType::CEventNetworkRemovedFromSessionDueToComplaints: {
			g_dxUiManager.sendNotify(xorstr_("Network"), xorstr_("You've been desync kicked"));
		} break;
		case rage::eEventNetworkType::CEventNetworkPlayerSpawn: {
			rage::sEventPlayerSpawn playerSpawn{};
			netEvent->get_extra_information(&playerSpawn, sizeof(playerSpawn));
			//g_dxUiManager.sendNotify(xorstr_("Network"), xorstr_("{} just spawned"), playerSpawn.m_name);
			if (util::player::g_handle->m_gamer_info.m_name == playerSpawn.m_name) {
				playerSpawn.pad_002C[9] = 'E';
				playerSpawn.pad_002C[10] = 'A';
				playerSpawn.pad_002C[11] = 'S';
			}
			if (playerSpawn.pad_002C[9] == 'E' && playerSpawn.pad_002C[10] == 'A' && playerSpawn.pad_002C[11] == 'S') {
				g_dxUiManager.sendNotify(xorstr_("Modder Detection"), xorstr_("{} is a modder (M[R]_ENT(164)::CENPS|M[M]_PC::FX)"), playerSpawn.m_name);
			}
		} break;
		case rage::eEventNetworkType::CEventNetworkPlayerCollectedAmbientPickup: {
			rage::sEventPlayerCollectedAmbientPickup* playerCollectedAmbientPickup{};
			netEvent->get_extra_information(playerCollectedAmbientPickup, sizeof(rage::sEventPlayerCollectedAmbientPickup));
			auto sender = util::network::getCNetGamePlayerViaConnectionId(playerCollectedAmbientPickup->m_player_index);
			switch (playerCollectedAmbientPickup->m_pickup_model) {
			case "vw_prop_vw_colle_alien"_joaat:
			case "vw_prop_vw_colle_imporage"_joaat:
			case "vw_prop_vw_colle_beast"_joaat:
			case "vw_prop_vw_colle_pogo"_joaat:
			case "vw_prop_vw_colle_prbubble"_joaat:
			case "vw_prop_vw_colle_rsrcomm"_joaat:
			case "vw_prop_vw_colle_rsrgeneric"_joaat:
			case "vw_prop_vw_colle_sasquatch"_joaat: {
				g_dxUiManager.sendNotify(xorstr_("Modder Detection"), xorstr_("{} is a modder (M[R]_ENT(166)::CENPCAP)"), sender->GetName());
				if (playerCollectedAmbientPickup->m_pickup_amount > 10) {
					g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Kick PU0 from {}"), sender->GetName());
					playerCollectedAmbientPickup->m_pickup_amount = 2;
					return;
				}
			} break;
			case "vw_prop_vw_lux_card_01a"_joaat: {
				g_dxUiManager.sendNotify(xorstr_("Modder Detection"), xorstr_("{} is a modder (M[R]_ENT(166)::CENPCAP)"), sender->GetName());
				if (playerCollectedAmbientPickup->m_pickup_amount > 2000) {
					g_dxUiManager.sendNotify(xorstr_("Event Protections"), xorstr_("Kick PU1 from {}"), sender->GetName());
					playerCollectedAmbientPickup->m_pickup_amount = 2;
					return;
				}
			} break;
			}
		} break;
		case rage::eEventNetworkType::CEventNetworkBail: {
			g_dxUiManager.sendNotify(xorstr_("Network"), xorstr_("NetBail was successfully called. Bail prevent fail"));
		} break;
		}
		RET_CALL(getNetworkEventData, _This, netEvent)
	}
}