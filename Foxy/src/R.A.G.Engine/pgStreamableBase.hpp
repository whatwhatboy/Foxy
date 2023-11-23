#pragma once
#include "sysUseAllocator.hpp"
#include "pgStreamManager.hpp"

namespace rage {
	class BlockMap;
	class pgStreamableBase : public rage::sysUseAllocator {
	public:
		inline void* operator new(size_t size, bool isPhysical) {
			void* data = pgStreamManager::Allocate(size, isPhysical, nullptr);
			memset(data, 0, size);
			return data;
		}
		inline void* operator new(size_t size, BlockMap* blockMap, bool isPhysical) {
			void* data = pgStreamManager::Allocate(size, isPhysical, blockMap);
			memset(data, 0, size);
			return data;
		}
		inline void* operator new[](size_t size) {
			return malloc(size);
		}
		inline void* operator new[](size_t size, bool isPhysical) {
			void* data = pgStreamManager::Allocate(size, isPhysical, nullptr);
			memset(data, 0, size);
			return data;
		}
		inline void* operator new[](size_t size, BlockMap* blockMap, bool isPhysical) {
			void* data = pgStreamManager::Allocate(size, isPhysical, blockMap);
			memset(data, 0, size);
			return data;
		}
	};
}