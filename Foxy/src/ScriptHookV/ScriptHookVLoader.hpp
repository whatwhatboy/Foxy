#pragma once
#include "ScriptHookV.hpp"
#include "Config/ScriptHookVConfig.hpp"
#include "Fibers/FiberMgr.hpp"

namespace base {
	class script {
	public:
		script(fs::path path) : m_path(path), m_name(path.filename().string()) {
			load();
		}
		~script() {
			free();
		}
	public:
		void load() {
			if (!m_loaded) {
				m_handle = LoadLibraryA(m_path.string().c_str());
				printf("loaded %s at %llx\n", m_name.c_str(), (uint64_t)m_handle);
				m_loaded = true;
			}
		}
		void free() {
			if (m_loaded && m_handle) {
				FreeLibrary(m_handle);
				m_loaded = false;
			}
		}
		std::string& str() {
			return m_name;
		}
		const char* c_str() {
			return m_name.c_str();
		}
		std::string fiber_id() {
			return std::format("0x{:X}", (uint64_t)m_handle);
		}
	private:
		bool m_loaded{};
		fs::path m_path{};
		std::string m_name{};
		HMODULE m_handle{};
	};
	class scriptHookV {
	public:
		void moduleLoad() {
			fs::path path{ std::getenv(xorstr_("appdata")) };
			path /= "Foxy";
			path /= "Modules";
			path /= "ScriptHookV.dll";
			LoadLibraryA(path.string().c_str());
		}
		void load(fs::path path) {
			m_scripts.push_back(new script(path.string()));
		}
		void unload(std::string name) {
			for (size_t i{ m_scripts.size() }; i; --i) {
				if (m_scripts[i]) {
					if (!name.compare(m_scripts[i]->c_str())) {
						g_fiberMgr.removeBase(m_scripts[i]->fiber_id().c_str());
						m_scripts.erase(m_scripts.begin() + i);
					}
				}
			}
		}
		void clear() {
			//Why does freeing fail? Possibly force dealloc ig.
			for (auto& s : m_scripts) {
				if (s) {
					g_fiberMgr.removeBase(s->fiber_id().c_str());
					s->free();
				}
			}
			m_scripts.clear();
		}
	private:
		std::vector<script*> m_scripts{};
		HMODULE m_module{};
	};
	inline scriptHookV g_scriptHookV{};
	class handshakeHandler {
	public:
		bool load() {
			g_SHVConfig.load();
			/*g_SHVConfig.get()["Running"] = true;
			g_SHVConfig.save();
			m_moduleAddress = (uint64_t)GetModuleHandleA("ScriptHookV.dll");
			m_exportModuleAddress = rage::atStringHash(util::localization::createRandomString(10));
			if (!handshakeSent())
				handle();*/
			return true;
		}
		void free() {
			g_SHVConfig.get()["ExportModuleAddress"] = 0ui64;
			g_SHVConfig.get()["HandshakeReceived"] = false;
			g_SHVConfig.get()["Running"] = false;
			g_SHVConfig.save();
		}
		bool handle() {
			if (sent() && moduleAddress()) {
				sendAsReceived();
				g_SHVConfig.save();
				handshakeSent() = true;
				return true;
			}
			return false;
		}
		bool& handshakeSent() {
			return m_handshakeSent;
		}
		HMODULE getExportModule() {
			return (HMODULE)m_exportModuleAddress;
		}
	private:
		void sendAsReceived() {
			g_SHVConfig.get()["ExportModuleAddress"] = m_exportModuleAddress;
			g_SHVConfig.get()["HandshakeReceived"] = true;
		}
		bool sent() {
			return g_SHVConfig.get()["HandshakeSent"].get<bool>();
		}
		uint64_t moduleAddress() {
			return g_SHVConfig.get()["ModuleAddress"].get<uint64_t>();
		}
		bool moduleValid() {
			return moduleAddress() && moduleAddress() == m_moduleAddress;
		}
	private:
		HMODULE m_module{};
		uint64_t m_moduleAddress{};
		uint64_t m_exportModuleAddress{};
		bool m_handshakeSent{};
	};
	inline handshakeHandler g_handshakeHandler{};
}