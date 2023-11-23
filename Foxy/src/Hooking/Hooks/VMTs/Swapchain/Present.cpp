#include "Hooking/Hooking.hpp"

namespace base {
	inline IUnknown* getBackbuffer(IDXGISwapChain* swap) {
		IUnknown* res = nullptr;
		swap->GetBuffer(0, IID_PPV_ARGS(&res));
		return res;
	}
	HRESULT hooks::present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
		g_cleanSurface = getBackbuffer(swapChain);
		if (g_renderer.get())
			g_renderer->present();
		return g_hooking->m_swapchainVftHk.getOg<decltype(&present)>(8)(swapChain, syncInterval, flags);
	}
}