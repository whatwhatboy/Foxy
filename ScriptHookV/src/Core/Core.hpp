#pragma once
#include "Pch/Common.hpp"

namespace Core {
	extern void Initialize();
	extern void Loop();
	extern void Uninitialize(HMODULE HMod);
}
namespace Thread {
	extern DWORD Entry(LPVOID Param);
	extern BOOL Create(HMODULE HMod, DWORD CallReason);
}