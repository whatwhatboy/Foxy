#include "Config.hpp"

namespace base {
	nlohmann::json g_dummyJson =
R"({
    "translationAuthor" : "Vali",
    "translationRevision" : "1",
    "translationLanguage" : "English",
	"Home" : {
		"opName" : "Home",
		"opTable" : {
			"Self" : {
				"opName" : "Self",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Weapons" : {
				"opName" : "Weapons",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Vehicle" : {
				"opName" : "Vehicle",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Network" : {
				"opName" : "Network",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Miscellaneous" : {
				"opName" : "Miscellaneous",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Protections" : {
				"opName" : "Protections",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Script Loader" : {
				"opName" : "Script Loader",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			},
			"Settings" : {
				"opName" : "Settings",
				"opType" : "sub",
				"opDesciption" : "",
				"opTable" : {}
			}
		},
		"opType": "sub"
	}
})"_json;
	bool config::load(std::string lang) {
		if (lang.empty()) {
			lang = "English";
		}
		m_path /= std::getenv(xorstr_("appdata"));
		m_path /= xorstr_(BRANDING_NAME);
		m_path /= xorstr_("Translations");
		if (!fs::exists(m_path))
			fs::create_directories(m_path);
		if (m_path.extension().string() != xorstr_(".json"))
			m_path /= lang + xorstr_(".json");
		if (m_currentLanguage != lang)
			m_currentLanguage = lang;
		std::ifstream m_stream{};
		m_stream.open(m_path);
		if (!m_stream.is_open()) {
			std::ofstream file;
			file.open(m_path, std::ios::out | std::ios::trunc);
			file << g_dummyJson.dump(1, '	');
			file.close();
			m_json.clear();
			m_json = g_dummyJson;;
			m_stream.open(m_path);
		}
		if (m_stream.is_open()) {
			m_stream >> m_json;
			m_stream.close();
			return true;
		}
		return false;
	}
	bool config::save() {
		std::ofstream file;
		file.open(m_path, std::ios::out | std::ios::trunc);
		file << m_json.dump(1, '	');
		file.close();
		return true;
	}
}