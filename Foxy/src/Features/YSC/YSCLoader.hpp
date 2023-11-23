#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#include "R.A.G.Engine/ScrProgram.hpp"
#include "R.A.G.Engine/ScrThread.hpp"
#include "R.A.G.Engine/Natives/Invoker/Invoker.hpp"
#include "R.A.G.Engine/Rage.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#include "YSCFile.hpp"

namespace base {
	namespace yscLoading {
		class scriptLoader {
		public:
			scriptLoader() : m_baseFilePath(fs::path(std::getenv("appdata")).append("Foxy\\Scripts")) {
			}
			~scriptLoader() {
				if (m_wasLoaded)
					restoreScript();
			}
		public:
			void restoreScript() {
			}
			void loadScript(char const* scrName) {
				auto path = fs::path(m_baseFilePath).append(scrName).string();
				yscFile file(path);
				file.load();
				m_program.m_code_blocks = file.m_codeBlockList.data();
				m_program.m_hash = rage::atStringHash(scrName);
				m_program.m_code_size = file.m_codeBlocks;
				m_program.m_arg_count = 0;
				m_program.m_local_count = 0;
				m_program.m_global_count = file.m_globalCount;
				m_program.m_native_count = file.m_nativesCount;
				m_program.m_local_data = nullptr;
				m_program.m_global_data = nullptr;
				m_program.m_natives = (rage::Cmd*)file.m_natives.data();
				m_program.m_proc_count = 0;
				m_program.m_proc_names = nullptr;
				m_program.m_name_hash = file.m_nameHash;
				m_program.m_ref_count = 0;
				m_program.m_name = file.m_name;
				m_program.m_string_heap = (const char*)file.m_stringBlockList.data();
				m_program.m_string_heap_size = file.m_stringBlocks;
				memset(m_program.m_breakpoints, NULL, sizeof(m_program.m_breakpoints));
				
				pointers::g_scriptVirtualMachine(m_thread.m_stack, pointers::g_globals, &m_program, &m_thread.m_context);
				m_wasLoaded = true;
			}
		public:
			rage::scrProgram m_program{};
			GtaThread m_thread{};
			bool m_wasLoaded{};
			fs::path m_baseFilePath{};
		};
		inline std::unique_ptr<scriptLoader> g_scriptLoader;
	}
}