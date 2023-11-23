#include "Hooking/Hooking.hpp"

namespace base {
	void hooks::copyResource(ID3D11DeviceContext* deviceContext, ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource) {
		if (pSrcResource == g_nonCleanSurface) {
			pSrcResource = (ID3D11Resource*)g_cleanSurface;
		}
		return g_hooking->m_d3dContextVftHk->getOg<decltype(&copyResource)>(9)(deviceContext, pDstResource, pSrcResource);
	}
}