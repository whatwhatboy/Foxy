#pragma once
#include "Pch/Common.hpp"
#include "Auth/XorStr.hpp"
#include "Auth/Encryption.hpp"
#include <winnt.h>
#include <winternl.h>
#include <shlwapi.h>
#include <TlHelp32.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "ntdll.lib")

namespace base::security {
	struct headerData { PIMAGE_DOS_HEADER dosHeaders; PIMAGE_NT_HEADERS ntHeaders; };
	inline std::map<HINSTANCE, headerData> g_headerData{};
	inline HMODULE g_ntdll{ LoadLibraryA(xorstr_("ntdll.dll")) };
	inline NTSTATUS(NTAPI* RtlAdjustPrivilege)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN){ decltype(RtlAdjustPrivilege)(GetProcAddress(g_ntdll, xorstr_("RtlAdjustPrivilege"))) };
	inline NTSTATUS(NTAPI* NtRaiseHardError)(NTSTATUS, ULONG, ULONG, PULONG_PTR, ULONG, PULONG){ decltype(NtRaiseHardError)(GetProcAddress(g_ntdll, xorstr_("NtRaiseHardError"))) };
	inline void bsod() {
		BOOLEAN bl;
		ULONG res;
		NTSTATUS NtRet = RtlAdjustPrivilege(19, TRUE, FALSE, &bl);
		NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, 0, 6, &res);
	}
	inline void eraseHeaders(HINSTANCE hMod) {
		if (!hMod)
			return;
		DWORD protect{};
		auto dosHeaders = PIMAGE_DOS_HEADER(hMod);
		auto ntHeaders = PIMAGE_NT_HEADERS(uintptr_t(hMod) + PIMAGE_DOS_HEADER(hMod)->e_lfanew);
		g_headerData.insert({ hMod, { dosHeaders, ntHeaders } });
		if (ntHeaders && VirtualProtect(ntHeaders, sizeof(ntHeaders), PAGE_EXECUTE_READWRITE, &protect))
			ZeroMemory(PVOID(ntHeaders), sizeof(ntHeaders));
		if (VirtualProtect(dosHeaders, sizeof(dosHeaders), PAGE_EXECUTE_READWRITE, &protect))
			ZeroMemory(PVOID(dosHeaders), sizeof(dosHeaders));
		return;
	}
	inline void restoreHeaders(HINSTANCE hMod) {
		if (!hMod)
			return;
		auto hd = g_headerData[hMod];
		DWORD protect{};
		auto dosHeaders = PIMAGE_DOS_HEADER(hMod);
		auto ntHeaders = PIMAGE_NT_HEADERS(uintptr_t(hMod) + PIMAGE_DOS_HEADER(hMod)->e_lfanew);
		if (hd.dosHeaders && hd.ntHeaders) {
			if (VirtualProtect(ntHeaders, sizeof(hd.ntHeaders), PAGE_EXECUTE_READWRITE, &protect))
				memcpy(PVOID(ntHeaders), PVOID(hd.ntHeaders), sizeof(hd.ntHeaders));
			if (VirtualProtect(dosHeaders, sizeof(hd.dosHeaders), PAGE_EXECUTE_READWRITE, &protect))
				memcpy(PVOID(dosHeaders), PVOID(hd.dosHeaders), sizeof(hd.dosHeaders));
		}
	}
	inline BOOL checkDebuggerPresent() {
		BOOL isDebugging = FALSE;
		CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebugging);
		return isDebugging;
	}
	inline bool hideThread(HANDLE thread) {
		auto status = NtSetInformationThread(!thread ? GetCurrentThread() : thread, (THREADINFOCLASS)17, 0, 0);
		if (status)
			return false;
		return true;
	}
	inline std::string hwid() {
		std::string tmp{};
		MEMORYSTATUSEX statex{ sizeof(statex) };
		GlobalMemoryStatusEx(&statex);
		auto totalPhyiscalSystemMemory = statex.ullTotalPhys / 1024;
		char volumeName[MAX_PATH + 1]{};
		char fileSystemName[MAX_PATH + 1]{};
		DWORD serialnumber{}, maxComponentLength{}, fileSystemFlags{};
		GetVolumeInformationA(xorstr_("C:\\"), volumeName, _ARRAYSIZE(volumeName), &serialnumber, &maxComponentLength, &fileSystemFlags, fileSystemName, _ARRAYSIZE(fileSystemName));
		std::string hwid{ std::format("{}{}{}{}", totalPhyiscalSystemMemory, serialnumber, fileSystemName, maxComponentLength) };
		char computerName[MAX_COMPUTERNAME_LENGTH + 1]{};
		DWORD size{ sizeof(computerName) / sizeof(computerName[0]) };
		GetComputerNameA(computerName, &size);
		return base64Encode(std::format("{}{}", hwid, computerName));
	}
}
namespace base {
	class auth {
	public:
		auth() = default;
		~auth() = default;
	public:
		void init();
	public:
		bool updateData(std::string data, std::string value);
		bool authenticate();
		bool ensureOnlineConnection();
	public:
		bool m_isLoggedIn{};
		#ifdef DEBUG_MODE
		char m_username[25]{ "Vali" };
		char m_password[128]{ "Test123" };
		#else
		char m_username[25]{};
		char m_password[128]{};
		#endif
	};
	inline auth g_auth;
}