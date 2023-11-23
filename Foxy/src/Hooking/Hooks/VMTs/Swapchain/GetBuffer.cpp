#include "Hooking/Hooking.hpp"

namespace base {
	HRESULT hooks::getBuffer(IDXGISwapChain* swapChain, UINT Buffer, REFIID riid, void** ppSurface) {
		auto ret = g_hooking->m_swapchainVftHk.getOg<decltype(&getBuffer)>(9)(swapChain, Buffer, riid, ppSurface);
		g_nonCleanSurface = *ppSurface;
		return ret;
	}
}