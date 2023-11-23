#include "Hooking/Hooking.hpp"
#include "Features/Features.hpp"

namespace base {
	void hooks::writePlayerGamerDataNode(rage::netObject* player, CPlayerGamerDataNode* pNode) {
		CALL(writePlayerGamerDataNode, player, pNode)
		auto& clanData = pNode->m_clan_data;
		if (auto& data = features::spoofing::clan::name; data.second) {
			strcpy(clanData.m_clan_name, data.first.c_str());
			if (data.first == "Rockstar")
				clanData.m_is_system_clan = true;
		}
		if (auto& data = features::spoofing::clan::tag; data.second) {
			strcpy(clanData.m_clan_tag, data.first.c_str());
		}
		if (auto& data = features::spoofing::clan::motto; data.second) {
			strcpy(clanData.m_clan_motto, data.first.c_str());
		}
		if (auto& data = features::spoofing::clan::rankName; data.second) {
			strcpy(clanData.m_clan_rank_name, data.first.c_str());
		}
		if (auto& data = features::spoofing::clan::rankTitle; data.second) {
			strcpy(pNode->m_crew_rank_title, data.first.c_str());
		}
		if (auto& data = features::spoofing::clan::color; data.second) {
			clanData.m_clan_color = data.first;
		}
		if (auto& data = features::spoofing::clan::clanId; data.second) {
			clanData.m_clan_id = data.first;
			clanData.m_clan_id_2 = data.first;
		}
		if (auto& data = features::spoofing::clan::creationTime; data.second) {
			clanData.m_clan_created_time = data.first;
		}
		if (auto& data = features::spoofing::clan::memberCount; data.second) {
			clanData.m_clan_member_id = data.first;
		}
		if (auto& data = features::spoofing::clan::memberCount; data.second) {
			clanData.m_clan_member_count = data.first;
		}
		clanData.m_is_clan_open = !features::spoofing::clan::privateCrew;
		pNode->m_is_rockstar_dev = features::spoofing::flags::rockstarDev;
		pNode->m_is_rockstar_qa = features::spoofing::flags::rockstarQA;
		pNode->m_is_cheater = features::spoofing::flags::isCheater;
	}
}