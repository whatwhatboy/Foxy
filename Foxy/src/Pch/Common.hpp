#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
//Windows Includes (SDKs)
#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <winternl.h>
#include <d3d11.h>
#include <Psapi.h>
#include <tlhelp32.h>
#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <type_traits>
#include <map>
#include <unordered_map>
#include <vector>
#include <array>
#include <wrl/client.h>
#include <random>
#include <filesystem>
#include <timeapi.h>
#include <span>
#include <set>
//DirectXTK Includes
#include <DirectXHelpers.h>
#include <DDSTextureLoader.h>
#include <CommonStates.h>
#include <Effects.h>
#include <GamePad.h>
#include <GeometricPrimitive.h>
#include <GraphicsMemory.h>
#include <Keyboard.h>
#include <Model.h>
#include <Mouse.h>
#include <PostProcess.h>
#include <PrimitiveBatch.h>
#include <ScreenGrab.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#define CURL_STATICLIB
//General Includes
#include <StackWalker.h>
//ImGui Includes
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
//Menu Macros
// Stringify Variables
#define STRINGIFY(x) #x
// Menu Information
//  Branding Names
//   Normal Branding Name
#define BRANDING_NAME "Foxy"
//   Uppercase Branding Name
#define UPPER_BRANDING_NAME "FOXY"
//   Lowercase Branding Name
#define LOWER_BRANDING_NAME "foxy"
//  Build Information
#define BUILD_NUMBER "30072"
#define VERSION_NUMBER "2.0.1"
#define BUILD_DATE __DATE__ " at " __TIME__
// Debug
#define DEBUG_MODE
// Disables exception handler for extensive debugging
#define DISABLE_EXCEPTION_HANDLERs
// Disables all auth related things, such as string encryption, debug protection, module altering, and pointer encryption
#ifdef DEBUG_MODE
	#define DISABLE_SECURITY
#endif
// Disables the translation system
#ifdef DEBUG_MODE
	#define DISABLE_TRANSLATIONS
#endif
// Disables the config system
#define DISBABLE_CONFIG_SECTION
//Ease Of Life Defines
// Using namespaces
using namespace std::chrono_literals;
// Shortened Namespaces
namespace fs = std::filesystem;
// Using defines
template <typename type>
using comPtr = Microsoft::WRL::ComPtr<type>;
template <typename t>
requires std::is_function_v<t>
using fnptr = std::add_pointer_t<t>;
// C++20 concepts
template <typename invokable, typename ...invokableArgs>
concept isInvokableWithArgs = requires(invokable callable, invokableArgs const& ...vaArgs) {
	callable(vaArgs...);
};
// Nullsub function
template <typename fT, typename ...args>
inline fT nullsub(args&&... arguments) {}
// Sizeof
#define SIZEOF(a) sizeof(a) / sizeof(std::remove_pointer_t<decltype(a)>)
// Count
#define COUNT(a) ((sizeof(a)/sizeof(0[a])) / ((size_t)(!(sizeof(a) % sizeof(0[a])))))
// Once
#define ONCE(a) do a while (false)
// Is Valid Pointer | Very cursed, but it works suprisingly well
template <typename t>
inline bool isValidPtr(t ptr) {
	auto address{ (uint64_t)(ptr) };
	if (address < 0x5000 || (address & 0xFFFFFFFF) == 0xFFFFFFFF || (address & 0xFFFFFFFF) <= 0xFF)
		return false;
	if (*(uint8_t*)((uint64_t)&address + 6) != NULL || *(uint8_t*)((uint64_t)&address + 7) != NULL)
		return false;
	return true;
}
// u8 String Copy
#define u8strcpy(dst, src, size) memcpy(dst, fs::path(src).u8string().c_str(), size)
// u16 String Copy
#define u16strcpy(dst, src, size) memcpy(dst, fs::path(src).u16string().c_str(), size)
// u32 String Copy
#define u32strcpy(dst, src, size) memcpy(dst, fs::path(src).u32string().c_str(), size)
// String Split
inline auto g_splitStr = [](std::string str, char split) -> std::vector<std::string> {
	std::vector<std::string> fields{};
	std::string field{};
	for (int i = 0; i < str.length(); i++) {
		if (str.at(i) == split) {
			fields.push_back(field);
			field.erase();
		}
		else {
			field += str.at(i);
			if (i == str.length() - 1)
				fields.push_back(field);
		}
	}
	return fields;
};
//Shortened Types
using st = size_t;
using u64 = uint64_t;
using s64 = int64_t;
using u32 = uint32_t;
using s32 = int32_t;
using u16 = uint16_t;
using s16 = int16_t;
using u8 = uint8_t;
using s8 = int8_t;
using cc = const char;
using ccp = cc*;
using buf_t = char*;
using fp = float;
using db = double;
//Base Defines
namespace base {
	inline HMODULE g_module{};
	inline bool g_running = true;
	inline int g_selectedVehicleClass{};
	inline int g_selectedVehicleFromClass{};
	inline int g_selectedPlayer{};
	inline int g_selectedFriend{};
	inline int g_selectedSession{};
	inline int g_selectedTooltip{};
	inline int g_selectedCustomTooltip{};
	inline fs::path g_filePath{};
	inline fs::path g_selectedTextureReplacement{};
	inline std::string g_selectedLua{};
	inline std::string g_selectedYSC{};
	inline std::string g_selectedImage{};
	inline bool isNumber(std::string str) {
		for (char const& c : str)
			if (std::isdigit(c) == 0)
				return false;
		return true;
	}
	inline bool containsANumber(std::string str) {
		for (char const& c : str)
			if (std::isdigit(c))
				return true;
		return false;
	}
}
//Helper classes
// StackWalker
class stackWalker : public StackWalker {
public:
	stackWalker() : StackWalker() {}
	stackWalker(ExceptType exType) : StackWalker(exType) {}
	virtual void OnOutput(LPCSTR szText);
	virtual void OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion);
	virtual void OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName);
	virtual void OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr);
	virtual void OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry);
};
// Random Number Generator
class rng {
	std::random_device m_rd{};
	std::mt19937 m_mtRes{};
public:
	template <typename t> requires std::is_integral_v<t>
		rng() : m_mtRes(m_rd()), m_min(std::numeric_limits<t>::min()), m_max(std::numeric_limits<t>::max()) {}
		rng(size_t min, size_t max) : m_mtRes(m_rd()), m_min(min), m_max(max) {}
public:
	template <typename t>
	t get() {
		std::uniform_int_distribution m_intDis(m_min, m_max);
		return static_cast<t>(m_intDis(m_mtRes));
	}
public:
	size_t m_min{};
	size_t m_max{};
};
// Stopwatch
class stopwatch {
public:
	stopwatch() : m_startTime(std::chrono::steady_clock::now()), m_endTime(std::chrono::steady_clock::now()) {}
public:
	template <typename t>
	t getAs() {
		m_endTime = std::chrono::steady_clock::now();
		return t(std::chrono::duration_cast<std::chrono::milliseconds>(m_endTime - m_startTime).count());
	}
public:
	std::chrono::steady_clock::time_point m_startTime;
	std::chrono::steady_clock::time_point m_endTime;
};