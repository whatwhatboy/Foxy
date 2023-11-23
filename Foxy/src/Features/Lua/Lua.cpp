#include "Lua.hpp"
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberMgr.hpp"
#include "Fibers/DXFiberMgr.hpp"
#include "Fibers/FiberPool.hpp"
#include "Fibers/Threading.hpp"
#include "R.A.G.Engine/Natives/Invoker/Invoker.hpp"
#include "DirectX/Renderer/Renderer.hpp"
#define SOL_IMGUI_USE_COLOR_U32
#include "ImGui/Lua.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "Util/Globals.hpp"
#include "Memory/Signature.hpp"
#include "Hooking/Hooking.hpp"

namespace base {
	void luaExecution::init() {
		//Register all required libraries
		m_luaState.open_libraries(sol::lib::base, sol::lib::package, sol::lib::coroutine, sol::lib::string, sol::lib::math, sol::lib::table);
		m_luaState.set_exception_handler(&exceptionHandler); //Set our exception handler to print of Lua errors
		//Call all register calbacks
		/*registerDatatypes();
		registerLogFunctions();
		registerScriptFunctions();
		registerInvokerFunctions();
		registerGlobalFunctions();
		registerMemoryFunctions();
		registerHookingFunctions();
		registerMiscFunctions();*/
		//Create ImGui functions on lua state
		sol_ImGui::Init(m_luaState);
		auto filePath = fs::path(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append(xorstr_("Scripts\\Lib"));
		if (fs::exists(filePath)) {
			fs::directory_iterator dirIt{ filePath.string() };
			static std::string hdrName{};
			for (auto&& dirEntry : dirIt) {
				if (dirEntry.is_regular_file()) {
					auto path = dirEntry.path();
					if (path.has_filename() && path.extension().string() == ".lua") {
						executeLua(path);
					}
				}
			}
		}
	}
	void luaExecution::executeLua(fs::path filePath) {
		sol::function_result grabFunctionResultFromLua = m_luaState.script_file(filePath.string(), &sol::script_pass_on_error);
		if (!grabFunctionResultFromLua.valid()) {
			sol::error err = grabFunctionResultFromLua;
			LOG(Warn, "Error {} was caught", err.what());
		}
	}
	void luaExecution::registerDatatypes() {
		/*m_luaState.new_usertype<Vector2>(			
			xorstr_("Vector2"), sol::constructors<Vector2(), Vector2(float, float)>(),
			xorstr_("x"), &Vector2::x,
			xorstr_("y"), &Vector2::y
		);
		m_luaState.new_usertype<Vector3>(
			xorstr_("Vector3"), sol::constructors<Vector3(), Vector3(float, float, float)>(),
			xorstr_("x"), &Vector3::x,
			xorstr_("y"), &Vector3::y,
			xorstr_("z"), &Vector3::z
		);*/
		/*auto col_t = m_luaState.new_usertype<Color>("Color", sol::constructors<Color(), Color(int, int, int, int)>());
		col_t["r"] = &Color::r, col_t["g"] = &Color::g, col_t["b"] = &Color::b, col_t["a"] = &Color::a;*/
		/*m_luaState.new_enum<true>(xorstr_("eLogColor"),
			xorstr_("Red"), eLogColor::Red,
			xorstr_("Green"), eLogColor::Green,
			xorstr_("Blue"), eLogColor::Blue,
			xorstr_("Intensify"), eLogColor::Intensify,
			xorstr_("White"), eLogColor::White,
			xorstr_("Grey"), eLogColor::Grey,
			xorstr_("LightRed"), eLogColor::LightRed,
			xorstr_("LightGreen"), eLogColor::LightGreen,
			xorstr_("LightBlue"), eLogColor::LightBlue
		);
		m_luaState.new_enum<true>(xorstr_("eLogType"),
			xorstr_("Info"), eLogType::Info,
			xorstr_("Warn"), eLogType::Warn,
			xorstr_("Fatal"), eLogType::Fatal,
			xorstr_("Exception"), eLogType::Exception,
			xorstr_("Registers"), eLogType::Registers,
			xorstr_("Backtrace"), eLogType::Backtrace,
			#ifdef DEBUG_MODE
				xorstr_("Debug"), eLogType::Debug,
			#endif
			xorstr_("Stackwalker"), eLogType::Stackwalker,
			xorstr_("Detours"), eLogType::Detours,
			xorstr_("File"), eLogType::File
		);*/
	}
	void luaExecution::registerLogFunctions() {
		auto logger = m_luaState[xorstr_("logger")].get_or_create<sol::table>();
		logger.set_function(xorstr_("log"), [=](eLogColor color, char const* type, char const* fmt, bool fileOnly) {
			g_logger.get()->log(color, type, fmt, fileOnly);
		});
		logger.set_function(xorstr_("log"), [=](eLogColor color, char const* type, char const* fmt) {
			g_logger.get()->log(color, type, fmt, false);
		});
		logger.set_function(xorstr_("log"), [=](eLogType type, char const* fmt) {
			g_logger.get()->log(type, fmt);
		});
	}
	void luaExecution::registerScriptFunctions() {
		auto script = m_luaState[xorstr_("script")].get_or_create<sol::table>();
		auto scriptMgr = m_luaState[xorstr_("scriptMgr")].get_or_create<sol::table>();
		auto dxMgr = m_luaState[xorstr_("dxMgr")].get_or_create<sol::table>();
		script.set_function(xorstr_("wait"), [=](int time) { fiber::cur()->wait(std::chrono::milliseconds(time)); });
		script.set_function(xorstr_("wait"), [=]() { fiber::cur()->wait(); });
		scriptMgr.set_function(xorstr_("add"), [=](sol::function fn, std::string name) {
			static sol::function fnCb{ fn };
			g_fiberMgr.add(std::string(xorstr_("lua_") + name).c_str(), [] {
				while (true) {
					fnCb();
					fiber::cur()->wait();
				}
			});
		});
		dxMgr.set_function(xorstr_("add"), [=](sol::function fn, std::string name) {
			static sol::function fnCb{ fn };
			g_dxFiberMgr.add(std::string(xorstr_("lua_") + name).c_str(), [] {
				while (true) {
					fnCb();
					fiber::cur()->wait();
				}
			});
		});
		scriptMgr.set_function(xorstr_("remove"), [=](std::string name) { g_fiberMgr.remove(std::string(xorstr_("lua_") + name).c_str()); });
		dxMgr.set_function(xorstr_("remove"), [=](std::string name) { g_dxFiberMgr.remove(std::string(xorstr_("lua_") + name).c_str()); });
	}
	template <typename t>
	t callNativeFromLua(sol::variadic_args args) {
		g_invoker.begin();
		for (size_t i{ 1 }; i != args.size(); ++i) {
			if (args[i].is<int>())
				g_invoker.push(args[i].as<int>());
			else if (args[i].is<float>())
				g_invoker.push(args[i].as<float>());
			else if (args[i].is<bool>())
				g_invoker.push(args[i].as<bool>());
			else if (args[i].is<const char*>())
				g_invoker.push(args[i].as<const char*>());
			else if (args[i].is<uintptr_t*>())
				g_invoker.push(args[i].as<uintptr_t*>());
			else if (args[i].is<Vector3>())
				g_invoker.push(args[i].as<Vector3>());
		}
		g_invoker.end(args[0].as<rage::scrNativeHash>());
		return g_invoker.getRet<t>();
	}
	void luaExecution::registerInvokerFunctions() {
		auto invoker = m_luaState[xorstr_("Native")].get_or_create<sol::table>();
		invoker.set_function(xorstr_("InvokeVoid"), [=](sol::variadic_args args) {
			return callNativeFromLua<Void>(args);
		});
		invoker.set_function(xorstr_("InvokeInt"), [=](sol::variadic_args args) {
			return callNativeFromLua<int>(args);
		});
		invoker.set_function(xorstr_("InvokeFloat"), [=](sol::variadic_args args) {
			return callNativeFromLua<float>(args);
		});
		invoker.set_function(xorstr_("InvokeBool"), [=](sol::variadic_args args) {
			return callNativeFromLua<bool>(args);
		});
		invoker.set_function(xorstr_("InvokeStr"), [=](sol::variadic_args args) {
			return callNativeFromLua<const char*>(args);
		});
		invoker.set_function(xorstr_("InvokePtr"), [=](sol::variadic_args args) {
			return callNativeFromLua<uintptr_t*>(args);
		});
		invoker.set_function(xorstr_("InvokeVec3"), [=](sol::variadic_args args) {
			return callNativeFromLua<Vector3>(args);
		});
	}
	void luaExecution::registerGlobalFunctions() {
		auto gbl = m_luaState[xorstr_("global")].get_or_create<sol::table>();
		auto local = m_luaState[xorstr_("local")].get_or_create<sol::table>();
		gbl.set_function(xorstr_("read"), [=](sol::variadic_args args) {
			return (decltype(args[1].get_type()))global(args[0].as<uint64_t>()).Value()->Any;
		});
		gbl.set_function(xorstr_("write"), [=](sol::variadic_args args) {
			auto type{ args[1].get_type() };
			global(args[0].as<uint64_t>()).Value()->Any = (size_t)args[1].as<decltype(type)>();
		});
		local.set_function(xorstr_("read"), [=](sol::variadic_args args) {
			auto thr{ args[0].is<char const*>() ? threading::getThr(args[0].as<char const*>()) : threading::getThrUsingHash(args[0].as<uint32_t>()) };
			return (decltype(args[1].get_type()))threadLocal(args[1].as<uint64_t>()).Value(thr->m_stack)->Any;
		});
		local.set_function(xorstr_("write"), [=](sol::variadic_args args) {
			auto type{ args[2].get_type() };
			auto thr{ args[0].is<char const*>() ? threading::getThr(args[0].as<char const*>()) : threading::getThrUsingHash(args[0].as<uint32_t>()) };
			threadLocal(args[1].as<uint64_t>()).Value(thr->m_stack)->Any = (size_t)args[2].as<decltype(type)>();
		});
	}
	void luaExecution::registerMemoryFunctions() {
		auto memory = m_luaState[xorstr_("memory")].get_or_create<sol::table>();
		memory.set_function(xorstr_("read"), [=](sol::variadic_args args) {
			if (args[0].as<uintptr_t>())
				return *(decltype(args[1].get_type())*)(args[0].as<uintptr_t>());
			return decltype(args[1].get_type())(NULL);
		});
		memory.set_function(xorstr_("write"), [=](sol::variadic_args args) {
			if (args[0].as<uintptr_t>()) {
				*(decltype(args[0].get_type())*)(args[0].as<uintptr_t>()) = args[1].as<decltype(args[1].get_type())>();
				return true;
			}
			return false;
		});
		memory.set_function(xorstr_("rip"), [=](char* address) {
			if (address)
				return memory::readRelative<uintptr_t>(address);
			return uintptr_t(0);
		});
		memory.set_function(xorstr_("scan"), [=](char const* address, std::string_view module) {
			if (sizeof(address) && !module.empty())
				return (char*)memory::signature(address).get(module);
			return (char*)-1i8;
		});
		memory.set_function(xorstr_("allocate"), [=](int size) {
			return std::make_unique<int>(size).get();
		});
	}
	void luaExecution::registerHookingFunctions() {
		//Hook callbacks
		//Currently supports: SGE, GCD
		auto hooking = m_luaState[xorstr_("hooking")].get_or_create<sol::table>();
		hooking.set_function(xorstr_("add"), [=](std::string id, int type, sol::function func) { g_hookQueue.add(id, type, func); });
		hooking.set_function(xorstr_("remove"), [=](std::string id) { g_hookQueue.remove(id); });
	}
	void luaExecution::registerMiscFunctions() {
		//True Misc
		m_luaState.set_function(xorstr_("playerBit"), [=](int player) { return Any(1 << player); });
		//WinAPI
		m_luaState.set_function(xorstr_("GetAsyncKeyState"), [=](int key) { return GetAsyncKeyState(key) != 0 ? true : false; });
		m_luaState.set_function(xorstr_("GetTickCount"), GetTickCount);
		m_luaState.set_function(xorstr_("GetTickCount64"), GetTickCount64);
		m_luaState.set_function(xorstr_("FindWindowA"), FindWindowA);
		m_luaState.set_function(xorstr_("GetForegroundWindow"), GetForegroundWindow);
	}
	int luaExecution::exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybeException, sol::string_view description) {
		auto whatError = (*maybeException).what();
		if (maybeException) {
			LOG(Warn, "Caught exception {}", whatError)
		}
		else {
			LOG(Warn, "Caught exception {} on {}", whatError, description.data())
		}
		return sol::stack::push(L, description);
	}
}