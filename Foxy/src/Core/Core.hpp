#pragma once
#include "Pch/Common.hpp"
#include "Logger.hpp"

namespace base::core {
	namespace dllMain {
		extern void dllAttach();
		extern void dllDetach();
		extern void createThreadInstance();
	}
	extern void init();
	extern void uninit();
}