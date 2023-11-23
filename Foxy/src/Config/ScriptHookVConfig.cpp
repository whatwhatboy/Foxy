#include "ScriptHookVConfig.hpp"
#include "Core/Logger.hpp"

namespace base {
	void SHVConfig::load() {
		bool shouldWrite{};
		m_path /= std::getenv(xorstr_("appdata"));
		m_path /= xorstr_("Foxy");
		m_path /= xorstr_("Cache");
		if (!fs::exists(m_path)) {
			fs::create_directories(m_path);
			shouldWrite = true;
		}
		if (!m_path.has_extension())
			m_path /= xorstr_("ScriptHookV.json");
		LOG_DEBUG("Path: {}", m_path.string());
		std::ifstream file(m_path, std::ios::in | std::ios::trunc);
		if (!file.is_open()) {
			if (shouldWrite)
				writeDummy();
			file.open(m_path);
		}
		std::stringstream str{};
		std::string line{};
		while (std::getline(file, line)) {
			str << line << '\n';
		}
		get() = nlohmann::json::parse(str.str());
	}
	void SHVConfig::save() {
		std::ofstream file;
		file.open(m_path, std::ios::out | std::ios::trunc);
		file << get().dump(1, '	');
		file.close();
	}
	void SHVConfig::writeDummy() {
		get() = g_dummy;
		save();
	}
}