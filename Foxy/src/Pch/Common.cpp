#include "Common.hpp"
#include "Core/Logger.hpp"
#include "Util/Util.hpp"

void stackWalker::OnOutput(LPCSTR szText) {
	std::string text(szText);
	LOG(Stackwalker, "{}", text.substr(0, text.size() - 1));
}
void stackWalker::OnLoadModule(LPCSTR img, LPCSTR mod, DWORD64 baseAddr, DWORD size, DWORD result, LPCSTR symType, LPCSTR pdbName, ULONGLONG fileVersion) {
#ifdef DEBUG_MODE
	StackWalker::OnLoadModule(img, mod, baseAddr, size, result, symType, pdbName, fileVersion);
#endif
}
void stackWalker::OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName) {
#ifdef DEBUG_MODE
	StackWalker::OnSymInit(szSearchPath, symOptions, szUserName);
#endif
}
void stackWalker::OnDbgHelpErr(LPCSTR szFuncName, DWORD gle, DWORD64 addr) {
	//std::string offsetStr{ base::util::modules::getModuleNameFromAddress<u64>(addr) };
	LOG(Stackwalker, "Error ({}) in {} at 0x{:X}", gle, szFuncName, addr);
}
void stackWalker::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry) {
	return StackWalker::OnCallstackEntry(eType, entry);
	std::string offsetStr{ std::format("{}.dll+0x{:X}", entry.moduleName, entry.offset) };
#ifdef DEBUG_MODE
	if (entry.lineFileName[0]) {
		if (entry.name[0]) {
			LOG(Stackwalker, "{} L{}: {} ({})", entry.lineFileName, entry.lineNumber, entry.name, offsetStr);
		}
	}
	else {
		if (entry.name[0]) {
			LOG(Stackwalker, "{} ({})", entry.name, offsetStr);
		}
		else {
			LOG(Stackwalker, "{}", offsetStr);
		}
	}
#else
	if (!entry.lineFileName[0]) {
		if (entry.name[0]) {
			LOG(Stackwalker, "{} ({})", entry.name, offsetStr);
		}
		else {
			LOG(Stackwalker, "{}", offsetStr);
		}
	}
#endif
}