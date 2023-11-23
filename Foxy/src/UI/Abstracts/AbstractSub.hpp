#pragma once
#include <string>
#include "UI/Abstracts/AbstractOpt.hpp"
#include "UI/Options/Opt.hpp"
#include "R.A.G.Engine/Joaat.hpp"

namespace base {
	struct subId {
	public:
		uint32_t id;
		std::string_view idStr;
	public:
		bool cmp(subId& subid) {
			return subid.id == id;
		}
		auto getId() {
			return id;
		}
		auto getIdAsString() {
			return std::string(idStr);
		}
		auto getSubTrail() {
			std::vector<std::string> splits{ g_splitStr(getIdAsString(), '.') };
			std::vector<std::string> trail{};
			for (int32_t i{ 0 }; i != splits.size(); ++i) {
				if (splits[i].find(xorstr_("_")) == std::string::npos) {
					auto str = splits[i];
					char beginStr[2] = { char(toupper(str.c_str()[0])), '\0' };
					trail.push_back(std::format("{}{}", beginStr, std::string(&(str.c_str()[1]))));
				}
				else {
					auto str = g_splitStr(splits[i], '_');
					char beginStr[2] = { char(toupper(str[0].c_str()[0])), '\0' };
					trail.push_back(std::format("{}{}{}{}", beginStr, std::string(&(str[0].c_str()[1])), str[0] == xorstr_("ammu") ? xorstr_("-") : xorstr_(" "), str[1]));
				}
			}
			for (auto& v : trail) {
				if (v.find(xorstr_("Script")) != std::string::npos || v.find(xorstr_("Ysc")) != std::string::npos || v.find(xorstr_("Ammu")) != std::string::npos) {
					if (!v.compare(xorstr_("Script loader"))) {
						v = xorstr_("Script Loader");
					}
					else if (!v.compare(xorstr_("Ysc"))) {
						v = xorstr_("YSC");
					}
					else if (!v.compare(xorstr_("Ammu-nation"))) {
						v = xorstr_("Ammu-Nation");
					}
					else if (!v.compare(xorstr_("Ip"))) {
						v = xorstr_("IP");
					}
					else if (!v.compare(xorstr_("Rid"))) {
						v = xorstr_("RID");
					}
					else if (!v.compare(xorstr_("Rid Tools"))) {
						v = xorstr_("RID Tools");
					}
					else if (!v.compare(xorstr_("Open tooltip"))) {
						v = xorstr_("Open Tooltip");
					}
					else if (!v.compare(xorstr_("Add tooltips"))) {
						v = xorstr_("Add Tooltips");
					}
					else if (!v.compare(xorstr_("All players"))) {
						v = xorstr_("All Players");
					}
					else if (!v.compare(xorstr_("Socket address"))) {
						v = xorstr_("Socket Address");
					}
					else if (!v.compare(xorstr_("Peer address"))) {
						v = xorstr_("Peer Address");
					}
					else if (!v.compare(xorstr_("Player count"))) {
						v = xorstr_("Player Count");
					}
					else if (!v.compare(xorstr_("Hash presets"))) {
						v = xorstr_("Hash Presets");
					}
					else if (!v.compare(xorstr_("Discord Rpc"))) {
						v = xorstr_("Discord RPC");
					}
					else if (!v.compare(xorstr_("Join requests"))) {
						v = xorstr_("Join Requests");
					}
					else if (!v.compare(xorstr_("Texture replacement"))) {
						v = xorstr_("Texture Replacement");
					}
					else if (!v.compare(xorstr_("Add replacement"))) {
						v = xorstr_("Add Replacement");
					}
					else if (!v.compare(xorstr_("Script events"))) {
						v = xorstr_("Script Events");
					}
					else if (!v.compare(xorstr_("Network events"))) {
						v = xorstr_("Network Events");
					}
					else if (!v.compare(xorstr_("Crash models"))) {
						v = xorstr_("Crash Models");
					}
					else if (!v.compare(xorstr_("Ui"))) {
						v = xorstr_("UI");
					}
					else if (!v.compare(xorstr_("Ne"))) {
						v = xorstr_("Network Events");
					}
				}
			}
			return trail;
		}
		void setData(nlohmann::json& json, std::string name, std::string desc, std::string type = xorstr_("sub"), std::string right = xorstr_("")) {
			if (json.is_null() || json.empty()) {
				if (!type.compare(xorstr_("sub"))) {
					json = {
						{ xorstr_("opName"), name },
						{ xorstr_("opDesciption"), desc.empty() ? xorstr_("") : desc },
						{ xorstr_("opType"), type },
						{ xorstr_("opTable"), {} },
					};
				}
				else if (type.find(xorstr_("Opt")) != std::string::npos) {
					json = {
						{ xorstr_("opName"), name },
						{ xorstr_("opRight"), right },
						{ xorstr_("opDesciption"), desc.empty() ? xorstr_("") : desc },
						{ xorstr_("opType"), type }
					};
				}
			}
		}
		//Retarded fucking json with it's retarded fucking nesting system, the main developer can shove a fat cock up his ass
		nlohmann::json& getJsonTrailAtIndex(size_t idx) {
			auto subTrail = getSubTrail();
			switch (idx) {
			case 1: {
				return g_config.m_json[subTrail[0]];
			} break;
			case 2: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]];
			} break;
			case 3: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]];
			} break;
			case 4: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]];
			} break;
			case 5: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]][xorstr_("opTable")][subTrail[4]];
			} break;
			case 6: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]][xorstr_("opTable")][subTrail[4]][xorstr_("opTable")][subTrail[5]];
			} break;
			case 7: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]][xorstr_("opTable")][subTrail[4]][xorstr_("opTable")][subTrail[5]];
			} break;
			case 8: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]][xorstr_("opTable")][subTrail[4]][xorstr_("opTable")][subTrail[5]][xorstr_("opTable")][subTrail[6]];
			} break;
			case 9: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]][xorstr_("opTable")][subTrail[4]][xorstr_("opTable")][subTrail[5]][xorstr_("opTable")][subTrail[6]][xorstr_("opTable")][subTrail[7]];
			} break;
			case 10: {
				return g_config.m_json[subTrail[0]][xorstr_("opTable")][subTrail[1]][xorstr_("opTable")][subTrail[2]][xorstr_("opTable")][subTrail[3]][xorstr_("opTable")][subTrail[4]][xorstr_("opTable")][subTrail[5]][xorstr_("opTable")][subTrail[6]][xorstr_("opTable")][subTrail[7]][xorstr_("opTable")][subTrail[8]];
			} break;
			}
		}
		nlohmann::json& getJson(std::string desc) {
			auto subTrail = getSubTrail();
			for (size_t i{ 1 }; i != subTrail.size() + 1; i++) {
				setData(getJsonTrailAtIndex(i), subTrail[i - 1], desc);
			}
			return getJsonTrailAtIndex(subTrail.size());
		}
		nlohmann::json& getJson(std::string desc, std::string right, std::string type) {
			auto subTrail = getSubTrail();
			for (size_t i{ 1 }; i != subTrail.size() + 1; i++) {
				setData(getJsonTrailAtIndex(i), subTrail[i - 1], desc, right, type);
			}
			return getJsonTrailAtIndex(subTrail.size());
		}
	};
	inline consteval subId operator ""_id(const char* str, size_t) {
		return { constexprJoaat(str), str };
	}
	namespace uiTypes {
		class abstractSub {
		public:
			//Deconstructor
			virtual ~abstractSub() = default;
			//Executing option(s)
			virtual void executeOptAdding() = 0;
			virtual void resetOptCount() = 0;

			//Getting sub id(s) and sub name(s)
			virtual std::string getName() = 0;
			virtual subId getId() = 0;
			virtual size_t getNumberOfOptions() = 0;
			virtual size_t getNumberOfBreakOptions() = 0;
			virtual size_t getSelectedOpt() = 0;

			virtual void setSelectedOpt(size_t opPos) = 0;
		public:
			void moveDown() {
				if (getSelectedOpt() < getNumberOfOptions() - 1)
					setSelectedOpt(getSelectedOpt() + 1);
				else
					setSelectedOpt(0);
			}
			void moveUp() {
				if (getSelectedOpt() > 0)
					setSelectedOpt(getSelectedOpt() - 1);
				else
					setSelectedOpt(getNumberOfOptions() - 1);
			}
		};
	}
}