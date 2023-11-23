#include "Pch/Common.hpp"
#include "Core/Core.hpp"

BOOL APIENTRY DllMain(HMODULE HMod, DWORD CallReason, LPVOID Reserved) {
    return Thread::Create(HMod, CallReason);
}