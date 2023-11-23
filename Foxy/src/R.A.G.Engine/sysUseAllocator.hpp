#pragma once
#include "SysMemAllocator.hpp"

namespace rage {
	class sysUseAllocator {
	public:
		void* operator new(size_t size) {
			return rage::sysMemAllocator::get()->Allocate(size, 16, NULL);
		}
		void* operator new[](size_t size) {
			return sysUseAllocator::operator new(size);
		}
		void operator delete(void* memory) {
			return rage::sysMemAllocator::get()->Free(memory);
		}
		void operator delete[](void* memory) {
			return sysUseAllocator::operator delete(memory);
		}
	};
}