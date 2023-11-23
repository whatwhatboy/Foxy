#pragma once
#include <cstdint>
#include "R.A.G.Engine/strStreamingModule.hpp"
#include "Memory/Patterns/Pointers.hpp"

namespace rage {
	class strStreamingModuleMgr {
	public:
		strStreamingModuleMgr() {}
		virtual ~strStreamingModuleMgr() = default;
		char pad_0008[16]; //0x0008
		rage::atArray<strStreamingModule*> m_modules; //0x0018

		strStreamingModule* GetModuleFromExtension(const char* extension) {
			return base::pointers::g_getStreamingModuleFromExtension(this, extension);
		}
		static strStreamingModuleMgr& Get();
	};
}