#pragma once
#include "UI/Options/RegularOpt.hpp"
#include "UI/Options/SubOpt.hpp"
#include "UI/Subs/Sub.hpp"
#include "UI/Options/BoolOpt.hpp"
#include "UI/Options/NumBoolOpt.hpp"
#include "UI/Options/NumOpt.hpp"
#include "UI/Options/VecOpt.hpp"
#include "UI/Options/VecBoolOpt.hpp"
#include "UI/Options/ColorOpt.hpp"
#include "UI/Options/BreakOpt.hpp"
#include "UI/Options/IconOpt.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "R.A.G.Engine/CVehicleModelInfo.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "Features/Features.hpp"
#include "Util/Util.hpp"
#include "Auth/XorStr.hpp"

namespace base::images {
	inline auto g_containsPicExtension = [](fs::path path) -> bool {
		auto ext = path.extension().string();
		std::vector<std::string> supportedFormats{ "png", "jpeg", "bmp", "psd", "tga", "gif", "hdr", "pic", "ppm", "pgm" };
		for (auto& format : supportedFormats) {
			if (ext == "." + format) {
				return true;
			}
		}
		return false;
	};
}
namespace base::models::dataStoring {
	inline std::map<int, std::vector<features::vehicleData>> g_cachedModelTable{};
	inline std::vector<std::string> g_modelClasses{};
	inline std::vector<features::vehicleData> modelDataSection(int modelClass) {
		std::vector<features::vehicleData> vehData{};
		auto modelTable = pointers::g_modelTable;
		for (int32_t i{}; i != modelTable->m_size; ++i) {
			for (auto node = modelTable->m_lookup_table[i]; node; node = node->m_next) {
				if (const auto tableIdx = node->m_idx; tableIdx != modelTable->m_size) {
					if (auto model = reinterpret_cast<rage::CVehicleModelInfo*>(modelTable->m_data[tableIdx]); model && (model->m_model_type & 0xFF) == (uint8_t)eModelType::Vehicle && (model->m_model_class & 0x1F) == modelClass) {
						std::string manufacturerLbl = util::localization::getGxtLabel(model->m_manufacturer, true);
						std::string nameLbl = util::localization::getGxtLabel(model->m_name, true);
						vehData.push_back({ model, manufacturerLbl, nameLbl.size() ? nameLbl : model->m_name });
					}
				}
			}
		}
		return vehData;
	}
	inline std::string getVehicleClassName(int32_t vehClass) {
		std::stringstream ss; ss << xorstr_("VEH_CLASS_") << vehClass;
		return util::localization::getGxtLabel(ss.str()).compare(xorstr_("NULL")) ? util::localization::getGxtLabel(ss.str()) : xorstr_("Unknown Class");
	}
	inline void getTable(int c, std::function<void(int, std::string, features::vehicleData)> cb) {
		for (int32_t i{}; i != g_cachedModelTable[c].size(); ++i) {
			cb(i, g_modelClasses[c], g_cachedModelTable[c][i]);
		}
	}
	inline features::vehicleData getDataFromIndex(int c, int i) {
		return g_cachedModelTable[c][i];
	}
	inline void cacheModelTable() {
		for (int32_t i{}; i != 22; ++i) {
			g_modelClasses.push_back(getVehicleClassName(i));
		}
		for (int32_t i{}; i != g_modelClasses.size(); ++i) {
			g_cachedModelTable.insert({ i, modelDataSection(i) });
		}
	}
}
namespace base {
	inline std::string getFriendStateStr(int state, bool joinable) {
		switch (state) {
		case 1: return xorstr_("[Online]"); break;
		case 2: return xorstr_("[Multiplayer]"); break;
		case 3: return joinable ? xorstr_("[Multiplayer]") : xorstr_("[Story]"); break;
		default: return xorstr_("[Offline]");
		}
		return xorstr_("null");
	}
}
namespace base {
	class mainScript {
	public:
		void initUi();
		static void tick();
	};
	inline mainScript g_mainScript;
}