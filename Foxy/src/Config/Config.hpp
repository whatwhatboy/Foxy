#pragma once
#include "Pch/Common.hpp"
#include "nlohmann/json.hpp"
#include "Auth/XorStr.hpp"

namespace base {
	class config {
	public:
		bool load(std::string lang);
		bool save();
	public:
		nlohmann::json m_json{};
	public:
		std::string m_currentLanguage{};
		fs::path m_path{};
	};
	inline config g_config{};
}