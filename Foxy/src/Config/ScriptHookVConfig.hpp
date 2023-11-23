#pragma once
#include "Pch/Common.hpp"
#include "nlohmann/json.hpp"
#include "Auth/XorStr.hpp"

namespace base {
	inline nlohmann::json g_dummy = R"({
	"Running" : true,
	"HandshakeSent" : false,
	"HandshakeReceived" : false,
	"ModuleAddress" : 0,
	"ExportModuleAddress" : 0
})"_json;
	class SHVConfig {
	public:
		void load();
		void save();
		void writeDummy();
	public:
		nlohmann::json& get() {
			return m_handle;
		}
	private:
		nlohmann::json m_handle{};
		fs::path m_path{};
	};
	inline SHVConfig g_SHVConfig{};
}