#include "Hooking/Hooking.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui_impl_dx11.h"

namespace base {
	HRESULT hooks::resizeBuffers(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {
		ImGui_ImplDX11_InvalidateDeviceObjects();
		auto hr = g_hooking->m_swapchainVftHk.getOg<decltype(&resizeBuffers)>(13)(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
		if (SUCCEEDED(hr)) //Why the fuck do we need this again? Oh wait, it's because of MS's shitty fucking HRESULT system
			ImGui_ImplDX11_CreateDeviceObjects();
		return hr;
	}
}