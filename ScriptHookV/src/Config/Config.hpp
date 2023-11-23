#pragma once
#include "Pch/Common.hpp"
#include "Json/Json.hpp"

inline nlohmann::json g_Dummy = R"({
	"Running" : true,
	"HandshakeSent" : false,
	"HandshakeReceived" : false,
	"ModuleAddress" : 0,
	"ExportModuleAddress" : 0
})"_json;
class Config {
public:
	void Load();
	void Save();
	void WriteDummy();
public:
	nlohmann::json& Get() {
		return m_Handle;
	}
	fs::path GetPath() {
		return m_Path;
	}
private:
	nlohmann::json m_Handle{};
	fs::path m_Path{ std::getenv("appdata") };
};
inline Config g_Config{};