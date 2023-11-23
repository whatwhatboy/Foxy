#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	bool hooks::writePlayerGameStateDataNode(rage::netObject* player, CPlayerGameStateDataNode* pNode) {
		auto res = CALL(writePlayerGameStateDataNode, player, pNode);
		//Set entity proofs
		if (features::spoofing::hideEntityProofs) {
			pNode->m_is_invincible = false;
			pNode->m_bullet_proof = false;
			pNode->m_melee_proof = false;
			pNode->m_steam_proof = false;
			pNode->m_explosion_proof = false;
			pNode->m_water_proof = false;
			pNode->m_fire_proof = false;
			pNode->m_collision_proof = false;
		}
		//Frame Flags
		pNode->m_super_jump = false;
		//Spectating
		if (features::spoofing::hidePlayerSpectating) {
			if (features::spectatePlayerBool[g_selectedPlayer])
				pNode->m_is_spectating = false;
			pNode->m_spectating_net_id = NULL;
		}
		return res;
	}
}