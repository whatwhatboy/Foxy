#pragma once
#include "Pch/Common.hpp"
#include "Auth/XorStr.hpp"

namespace base {
	enum class eLogColor : uint16_t {
		Red = FOREGROUND_RED,
		Green = FOREGROUND_GREEN,
		Blue = FOREGROUND_BLUE,
		Intensify = FOREGROUND_INTENSITY,
		White = Red | Green | Blue | Intensify,
		Grey = Intensify,
		LightRed = Red | Intensify,
		LightGreen = Green | Intensify,
		LightBlue = Blue | Intensify,
	};
	enum class eLogType : uint8_t {
		Info,
		Warn,
		Fatal,
		Exception,
		Registers,
		Backtrace,
		#ifdef DEBUG_MODE
		Debug,
		#endif
		Stackwalker,
		Detours,
		File
	};
	inline eLogColor operator|(eLogColor a, eLogColor b) {
		return static_cast<eLogColor>(static_cast<std::underlying_type_t<eLogColor>>(a) | static_cast<std::underlying_type_t<eLogColor>>(b));
	}
	class logger {
	public:
		logger() {
			initConsoleHandles();
			initFileHandles();
		}
		~logger() {
			freeConsoleHandles();
			freeFileHandles();
		}
	public:
		void initConsoleHandles() {
			if (!AttachConsole(GetCurrentProcessId()))
				AllocConsole();
			SetConsoleTitleA(xorstr_(BRANDING_NAME));
			freopen_s((FILE**)stdout, xorstr_("CONOUT$"), xorstr_("w"), stdout);
			SetConsoleCP(CP_UTF8);
			SetConsoleOutputCP(CP_UTF8);
		}
		void initFileHandles() {
			m_console.open(xorstr_("CONOUT$"));
			m_path.append(std::getenv(xorstr_("appdata"))).append(xorstr_(BRANDING_NAME)).append(xorstr_("Log.txt"));
			m_file.open(m_path, std::ios_base::out | std::ios_base::trunc);
		}
	public:
		void freeConsoleHandles() {
			fclose(stdout);
			FreeConsole();
		}
		void freeFileHandles() {
			m_console.clear();
			m_console.close();
			m_path.clear();
			m_file.close();
		}
	public:
		template <typename ...arguments>
		void log(eLogColor col, std::string type, std::string fmt, bool fileOnly, arguments... args) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<uint16_t>(col));
			auto timeSinceEpoch = std::time(nullptr);
			auto timeStruct = std::localtime(&timeSinceEpoch);
			auto timestamp = std::format("{:0>2}:{:0>2}:{:0>2}", timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec);
			auto msg = std::vformat(fmt, std::make_format_args(args...));
			if (!fileOnly)
				m_console << timestamp << xorstr_(" | ") << type << xorstr_(" | ") << msg << std::endl;
			m_file << xorstr_("[") << timestamp << xorstr_("] ") << msg << '\n';
		}
		template <typename ...arguments>
		void log(eLogType type, std::string fmt, arguments... args) {
			switch (type) {
			case eLogType::Info: log(eLogColor::White, xorstr_("Info"), fmt, false, args...); break;
			case eLogType::Warn: log(eLogColor::LightBlue, xorstr_("Warning"), fmt, false, args...); break;
			case eLogType::Fatal: log(eLogColor::Red, xorstr_("Fatal"), fmt, false, args...); break;
			case eLogType::Exception: log(eLogColor::LightRed, xorstr_("Exception"), fmt, false, args...); break;
			case eLogType::Registers: log(eLogColor::Grey, xorstr_("Registers"), fmt, false, args...); break;
			case eLogType::Backtrace: log(eLogColor::Grey, xorstr_("Backtrace"), fmt, false, args...); break;
			#if defined(DEBUG_MODE)
				case eLogType::Debug: log(eLogColor::Blue, xorstr_("Debug"), fmt, false, args...); break;
			#endif
			case eLogType::Stackwalker: log(eLogColor::Grey, xorstr_("StackWalker"), fmt, false, args...); break;
			case eLogType::Detours: log(eLogColor::Red, xorstr_("Hooking"), fmt, false, args...); break;
			case eLogType::File: log(eLogColor::Intensify, xorstr_("File"), fmt, true, args...); break;
			}
		}
	public:
		fs::path m_path{};
		std::ofstream m_file{};
		std::ofstream m_console{};
	};
	inline std::unique_ptr<logger> g_logger{};
}

#define LOG(type, value, ...) base::g_logger.get()->log(base::eLogType::type, xorstr_(value), __VA_ARGS__);
#define LOG_UNENCRYPTED(type, value, ...) base::g_logger.get()->log(base::eLogType::type, value, __VA_ARGS__);
#define LOG_DIRECT(type, value, ...) base::g_logger.get()->log(type, xorstr_(value), __VA_ARGS__);

#ifdef DEBUG_MODE
	#define LOG_DEBUG(value, ...) LOG(Debug, value, __VA_ARGS__)
#else
	#define LOG_DEBUG(value, ...) ;
#endif