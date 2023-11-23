#pragma once
#include "Pch/Common.hpp"
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

namespace base {
	class luaExecution {
	public:
		luaExecution() = default;
		~luaExecution() = default;
	public:
		void init();
	public:
		void executeLua(fs::path filePath);
	public:
		void registerDatatypes();
		void registerLogFunctions();
		void registerScriptFunctions();
		void registerInvokerFunctions();
		void registerGlobalFunctions();
		void registerMemoryFunctions();
		void registerHookingFunctions();
		void registerMiscFunctions();
	public:
		static int exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybeException, sol::string_view description);
	public:
		sol::state m_luaState;
	};
	inline luaExecution g_lua;
}