#include "Pch/Common.hpp"
#include "Config.hpp"

void Config::Load() {
	m_Path = fs::path();
	m_Path /= std::getenv("appdata");
	m_Path /= "Foxy";
	m_Path /= "Cache";
	if (!fs::exists(m_Path)) {
		fs::create_directories(m_Path);
	}
	if (!m_Path.has_extension())
		m_Path /= "ScriptHookV.json";
	std::ifstream File(m_Path, std::ios::in | std::ios::trunc);
	if (!File.is_open()) {
		//WriteDummy();
		File.open(m_Path);
	}
	std::stringstream Str{};
	std::string Line{};
	while (std::getline(File, Line)) {
		Str << Line << '\n';
	}
	File.close();
	Get() = nlohmann::json::parse(Str.str());
}
void Config::Save() {
	std::ofstream file(m_Path, std::ios::out | std::ios::trunc);
	file << Get().dump(1, '	');
	file.close();
}
void Config::WriteDummy() {
	Get() = g_Dummy;
	Save();
}