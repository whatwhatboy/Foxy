#pragma once
#include "Pch/Common.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "Features/Features.hpp"
#include "R.A.G.Engine/NetPlayer.hpp"
#include "R.A.G.Engine/CNetGamePlayer.hpp"

namespace base::cmdHelpers {
	inline std::string getWordAtIdx(std::vector<std::string> list, int atIndex) {
		if (list.size() > atIndex)
			return list.at(atIndex);
		return "null";
	}
	inline int8_t nameToPid(std::string name) {
		auto netPlyrMgr = *pointers::g_networkPlayerMgr;
		for (auto plyr : netPlyrMgr->m_player_list) {
			if (plyr != nullptr) {
				if (plyr->IsConnected()) {
					if (plyr->GetName() == name.c_str()) {
						return plyr->m_player_id;
					}
				}
			}
		}
		return -1;
	}
	extern void helpFunc(int player, std::vector<std::string> wordArr);
	extern void printFunc(int player, std::vector<std::string> wordArr);
	extern void nameToPidFunc(int player, std::vector<std::string> wordArr);
	extern void pidToNameFunc(int player, std::vector<std::string> wordArr);
	extern void spawnFunc(int player, std::vector<std::string> wordArr);
}
namespace base {
	struct chatCmdPlyr { int index; std::string cmdName; std::string cmdUsage; std::string cmdInfo; std::function<void(int, std::vector<std::string>)> cmdFunc; bool& toggle; };
	template <size_t i>
	class chatCmds {
	public:
		void restoreCmds() {
			for (auto& cmd : cmdMap)
				cmd.isAllowedAccess[i] = false;
		}
	public:
		bool m_hasCmd[i]{};
		std::string cmdPrefix = ".";
	public:
		std::vector<chatCmdPlyr> cmdMap = {
			{ 0, "help", "help (no given paramaters)", "Prints helpful information about commands", cmdHelpers::helpFunc, m_hasCmd[0] },
			{ 1, "print", "print (name, showToThem)", "Prints a message as somneone else, allows you to make it so they can't see it", cmdHelpers::printFunc, m_hasCmd[1] },
			{ 2, "nametopid", "pidtoname (name)", "The CMD prints the PID corresponding to the given name.", cmdHelpers::nameToPidFunc, m_hasCmd[2] },
			{ 3, "pidtoname", "pidtoname (pid)", "The CMD prints the name corresponding to the given PID.", cmdHelpers::pidToNameFunc, m_hasCmd[3] },
			{ 4, "spawn", "spawn (hashkey)", "Spawns something using the given hashkey. Allows vehicle, object, and ped spawning (uses streaming checks).", cmdHelpers::spawnFunc, m_hasCmd[4] },
		};
	};
	inline chatCmds<5> g_cmdArr[32];
	inline void handleCmds(int plyrId, std::vector<std::string> msgArr) {
		auto cmdMap = g_cmdArr[plyrId].cmdMap;
		for (auto cmdIdx = 0; cmdIdx < cmdMap.size(); cmdIdx++) {
			if (cmdHelpers::getWordAtIdx(msgArr, 0) == g_cmdArr[plyrId].cmdPrefix + cmdMap[cmdIdx].cmdName) {
				if (cmdMap[cmdIdx].toggle)
					cmdMap[cmdIdx].cmdFunc(plyrId, msgArr);
			}
		}
	}
}
namespace base::cmdHelpers {
	static std::string msgAtIdx{};
	inline void helpFunc(int player, std::vector<std::string> wordArr) {
		for (auto i = 0; i < 31; i++) {
			if (i != player)
				features::msgSyncWithPerson[i] = true;
		}
		features::msgSyncWithPerson[player] = false;
		std::string cmds{};
		auto cmdMap = g_cmdArr[player].cmdMap;
		for (auto cmdIdx = 0; cmdIdx < cmdMap.size(); cmdIdx++) {
			if (cmdMap[cmdIdx].toggle)
				cmds += " " + cmdMap[cmdIdx].cmdName;
		}
		if (!cmds.empty()) {
			auto words = getWords(cmds);
			cmds.clear();
			auto firstWrd = words[0];
			/*auto lastWrd = words[words.size()];
			cmds += firstWrd;
			for (auto& wrd : words)
				if (wrd != firstWrd && wrd != lastWrd)
					cmds += wrd + ", ";
			cmds += lastWrd;*/
			features::chatSender(std::format("(This is a isolated message) You have access to the following commands: {}", cmds), false);
			for (auto cmdIdx = 0; cmdIdx < cmdMap.size(); cmdIdx++) {
				if (cmdMap[cmdIdx].toggle)
					features::chatSender(std::format("Command: {}, usage: {}, what it does: {}", cmdMap[cmdIdx].cmdName, cmdMap[cmdIdx].cmdUsage, cmdMap[cmdIdx].cmdInfo), false);
			}
		}
	}
	inline void printFunc(int player, std::vector<std::string> wordArr) {
		msgAtIdx = getWordAtIdx(wordArr, 1);
		features::personToSpoofAs = nameToPid(msgAtIdx);
		static bool wasEnabled = true;
		if (getWordAtIdx(wordArr, 2) == "true")
			if (!features::msgSyncWithPerson[player]) {
				wasEnabled = false;
				features::msgSyncWithPerson[player] = true;
			}
		for (auto& wrd : wordArr)
			if (wrd != getWordAtIdx(wordArr, 0) && wrd != msgAtIdx)
				features::chatSender(std::format("{}", wrd), false);
		if (!wasEnabled)
			features::msgSyncWithPerson[player] = false;
		features::personToSpoofAs = PLAYER::PLAYER_ID();
	}
	inline void nameToPidFunc(int player, std::vector<std::string> wordArr) {
		msgAtIdx = getWordAtIdx(wordArr, 1);
		features::personToSpoofAs = player;
		features::chatSender(std::format("Name {} -> PID {}", msgAtIdx, nameToPid(msgAtIdx)), false);
		features::personToSpoofAs = PLAYER::PLAYER_ID();
	}
	inline void pidToNameFunc(int player, std::vector<std::string> wordArr) {
		msgAtIdx = getWordAtIdx(wordArr, 1);
		auto netPlyrMgr = *pointers::g_networkPlayerMgr;
		for (auto plyr : netPlyrMgr->m_player_list) {
			if (plyr != nullptr) {
				if (plyr->IsConnected()) {
					if (plyr->m_player_id == stoi(msgAtIdx)) {
						features::personToSpoofAs = player;
						features::chatSender(std::format("PID {} -> Name {}", plyr->m_player_id, plyr->GetName()), false);
						features::personToSpoofAs = PLAYER::PLAYER_ID();
					}
				}
			}
		}
	}
	inline void spawnFunc(int player, std::vector<std::string> wordArr) {
		msgAtIdx = getWordAtIdx(wordArr, 1);
		features::personToSpoofAs = player;
		g_fiberPool.queue([] {
			if (!STREAMING::IS_MODEL_VALID(rage::atStringHash(msgAtIdx))) {
				if (!isNumber(msgAtIdx)) {
					/*if (auto x = stoul(msgAtIdx, nullptr, 16); x) {
						if (STREAMING::IS_MODEL_A_VEHICLE(x)) {
							auto veh = features::spawnVehicle(features::personToSpoofAs, x);
							features::setNetParamsVeh(veh);
						}
						else if (STREAMING::IS_MODEL_A_PED(x)) {
							auto ped = features::spawnPed(features::personToSpoofAs, x);
							features::setNetParamsPed(ped);
						}
						else {
							auto obj = features::spawnObject(features::personToSpoofAs, x);
							features::setNetParamsObj(obj);
						}
					}*/
				}
				else {
					features::chatSender(std::format("Sorry! {} isn't a valid model, please refrence gta-objects.xyz/", msgAtIdx), false);
				}
			}
			else if (STREAMING::IS_MODEL_A_VEHICLE(rage::atStringHash(msgAtIdx))) {
				auto veh = features::spawnVehicle(features::personToSpoofAs, rage::atStringHash(msgAtIdx));
				features::setNetParamsVeh(veh);
			}
			else if (STREAMING::IS_MODEL_A_PED(rage::atStringHash(msgAtIdx))) {
				auto ped = features::spawnPed(features::personToSpoofAs, rage::atStringHash(msgAtIdx));
				features::setNetParamsPed(ped);
			}
			else {
				auto obj = features::spawnObj(features::personToSpoofAs, rage::atStringHash(msgAtIdx));
				features::setNetParamsObj(obj);
			}
		});
		features::personToSpoofAs = PLAYER::PLAYER_ID();
	}
}