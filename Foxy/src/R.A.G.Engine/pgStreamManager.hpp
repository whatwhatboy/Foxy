#pragma once
#include <cstdint>
#include <set>
#include "BlockMap.hpp"
#include <unordered_set>
#include <Core/Logger.hpp>

namespace rage {
	class pgPtrRepresentation {
	public:
		uint64_t m_pointer : 28; //0x0000
		uint64_t m_block_type : 36; //0x0008
	}; //Size: 0x0010
	static __declspec(thread) BlockMap* g_currentBlockMap;
	static __declspec(thread) BlockMap* g_packBlockMap;
	static __declspec(thread) std::vector<std::tuple<pgPtrRepresentation*, bool, void*>>* g_packEntries;
	static __declspec(thread) std::set<pgPtrRepresentation*>* g_packPointerSet;
	class BlockMapGap {
	public:
		char* start; //0x0000
		size_t size; //0x0008
	}; //Size: 0x0010
	class BlockMapMeta {
	public:
		size_t maxSizes[128]; //0x0000
		size_t realMaxSizes[128];
		std::vector<BlockMapGap> gapList[2];
		bool isPerformingFinalAllocation;
		std::list<std::tuple<void*, size_t, bool>> allocations;
		size_t baseMemorySize;
	}; //Size: 0x0000
	static std::unordered_map<BlockMap*, BlockMapMeta> g_allocationData;
	static std::unordered_set<const void*> g_resolvedEntries;
	class pgStreamManager {
	public:
		static void* ResolveFilePointer(pgPtrRepresentation& ptr, BlockMap* blockMap = nullptr) {
			if (ptr.m_block_type == 0) {
				return nullptr;
			}
			if (!blockMap) {
				blockMap = g_currentBlockMap;
			}
			int startIdx{ ptr.m_block_type == 5 ? 0 : blockMap->m_virtual_len };
			int endIdx{ ptr.m_block_type == 5 ? blockMap->m_virtual_len : (blockMap->m_virtual_len + blockMap->m_physical_len) };
			for (int i{ startIdx }; i != endIdx; ++i) {
				auto& block = blockMap->m_blocks[i];
				if (ptr.m_pointer >= block.offset && ptr.m_pointer < (block.offset + block.size)) {
					return (char*)block.data + (ptr.m_pointer - block.offset);
				}
			}
		}
		static void SetBlockInfo(BlockMap* blockMap) {
			g_currentBlockMap = blockMap;
		}
		static bool IsResolved(const void* ptrAddress) {
			return g_resolvedEntries.find(ptrAddress) != g_resolvedEntries.end();
		}
		static void MarkResolved(const void* ptrAddress) {
			g_resolvedEntries.insert(ptrAddress);
		}
		static void UnmarkResolved(const void* ptrAddress) {
			g_resolvedEntries.erase(ptrAddress);
		}
		static BlockMap* BeginPacking() {
			auto newMap = CreateBlockMap();
			BeginPacking(newMap);
			return newMap;
		}
		static BlockMap* CreateBlockMap() {
			auto newMap = new BlockMap();
			memset(newMap, 0, sizeof(BlockMap));
			BlockMapMeta meta = { 0 };
			g_allocationData[newMap] = meta;
			return newMap;
		}
		static void DeleteBlockMap(BlockMap* blockMap) {
			for (int i = 0; i < blockMap->m_physical_len + blockMap->m_virtual_len; i++) {
				free(blockMap->m_blocks[i].data);
			}
			g_allocationData.erase(blockMap);
			delete blockMap;
		}
		static void BeginPacking(BlockMap* blockMap) {
			if (g_packEntries != nullptr) {
				delete g_packEntries;
			}
			g_packEntries = new std::vector<std::tuple<pgPtrRepresentation*, bool, void*>>();
			g_packPointerSet = new std::set<pgPtrRepresentation*>();
			g_packBlockMap = blockMap;
		}
		static void MarkToBePacked(pgPtrRepresentation* ptrRepresentation, bool physical, void* tag) {
			if (!g_packBlockMap) {
				return;
			}
			//Check if the block is in the block map
			auto inPtr = *(uintptr_t*)ptrRepresentation;
			if (!IsInBlockMap((void*)inPtr, g_packBlockMap, physical)) {
				return;
			}
			//Physical typically doesn't contain pointers to other physicals, so we use false here
			if (!IsInBlockMap(ptrRepresentation, g_packBlockMap, false)) {
				return;
			}
			//Add the block
			g_packPointerSet->insert(ptrRepresentation);
			g_packEntries->push_back(std::make_tuple(ptrRepresentation, physical, tag));
		}
		static void EndPacking() {
			FinalizeAllocations(g_packBlockMap);
			g_resolvedEntries.clear();
			delete g_packEntries;
			g_packEntries = nullptr;
		}
		static void FinalizeAllocations(BlockMap* blockMap) {
			//Find an allocation block for this block map
			auto allocBlock = g_allocationData.find(blockMap);
			if (allocBlock == g_allocationData.end()) {
				return;
			}
			auto& allocInfo = allocBlock->second;
			allocInfo.isPerformingFinalAllocation = true;
			auto& allocations = allocInfo.allocations;
			//Get the first allocation (to not mess with it at any later stage)
			auto firstAlloc = allocations.front();
			allocations.pop_front();
			//Sort the remaining allocations by size - the biggest goes first (to allow gap-based allocation of pages at a later time)
			std::vector<std::tuple<void*, size_t, bool>> sortedAllocations(allocations.begin(), allocations.end());
			std::sort(sortedAllocations.begin(), sortedAllocations.end(), [](const auto& left, const auto& right) {
				if (std::get<bool>(left) == std::get<bool>(right)) {
					return (std::get<size_t>(left) > std::get<size_t>(right));
				}
				return std::get<bool>(left) < std::get<bool>(right);
			});
			//Configure an ideal allocation base - this could get messy; as we'll allocate/free the block map a *lot* of times
			std::vector<void*> curAllocatedPtrs{};
			BlockMap* curBlockMap{};
			auto attemptAllocation = [&](size_t base, bool physical) {
				//Create a new block map and a metadata set for it
				auto bm{ CreateBlockMap() };
				bm->m_base_allocation_size[physical] = base;
				auto allocBlock = g_allocationData.find(bm);
				auto& allocInfo = allocBlock->second;
				allocInfo.isPerformingFinalAllocation = true;
				allocInfo.baseMemorySize = base;
				std::vector<void*> allocatedPtrs{};
				bool fitting{ true };
				if (!physical) {
					void* ptr{ Allocate(std::get<1>(firstAlloc), false, bm) };
					if (ptr) {
						allocatedPtrs.push_back(ptr);
					}
				}
				for (auto& alloc : sortedAllocations) {
					if (std::get<bool>(alloc) == physical) {
						void* ptr{ Allocate(std::get<1>(alloc), std::get<2>(alloc), bm) };
						if (!ptr) {
							fitting = false;
							break;
						}
						allocatedPtrs.push_back(ptr);
					}
				}
				if (!fitting) {
					DeleteBlockMap(bm);
					bm = nullptr;
					allocatedPtrs.clear();
				}
				return std::pair<BlockMap*, std::vector<void*>>(bm, allocatedPtrs);
			};
			{
				BlockMap* blockMaps[2]{ nullptr, nullptr };
				std::vector<void*> ptrLists[2]{};
				while (!blockMaps[1]) {
					size_t bestTotalMemory = -1;
					bool physical = (blockMaps[0] != nullptr);
					BlockMap*& curBlockMapRef = *&blockMaps[physical];
					std::vector<void*>& curAllocatedPtrsRef = *&ptrLists[physical];
					for (size_t i{}; i != 16; ++i) {
						// attempt to allocate a block map set for the base
						size_t newBase{ (1ui64 << i) << 13ui64 };
						auto pair{ attemptAllocation(newBase, physical) };
						//If allocation failed, continue
						if (pair.first == nullptr) {
							continue;
						}
						//Count the total in-memory size
						size_t memorySize{};
						const int8_t maxMults[] = { 16, 8, 4, 2, 1 };
						const uint8_t maxCounts[] = { 1, 3, 15, 63, 127 };
						auto getSize = [&](int first, int count) {
							int last{ first + count };
							int curMult{};
							int curCount{};
							size_t lastSize{};
							size_t thisSize{};
							for (int j{ first }; j != last; ++j) {
								thisSize += lastSize = pair.first->m_blocks[j].size;
								curCount++;
								if (curCount >= maxCounts[curMult]) {
									curMult++;
									curCount = 0;
								}
							}
							if (lastSize > 0) {
								for (int j{ _countof(maxMults) - 1 }; j >= 0; --j) {
									size_t nextSize = (maxMults[j] >= 0) ? (newBase * maxMults[j]) : (newBase / -maxMults[j]);
									if (lastSize <= nextSize) {
										thisSize += (nextSize - lastSize);
										break;
									}
								}
							}
							return thisSize;
						};
						memorySize += getSize(0, pair.first->m_virtual_len);
						memorySize += getSize(pair.first->m_virtual_len, pair.first->m_physical_len);
						if (memorySize < bestTotalMemory) {
							if (curBlockMapRef) {
								DeleteBlockMap(curBlockMapRef);
							}
							curBlockMapRef = pair.first;
							curAllocatedPtrsRef = pair.second;
							bestTotalMemory = memorySize;
						}
						else {
							DeleteBlockMap(pair.first);
						}
						if (memorySize > bestTotalMemory) {
							//Probably not going to get any better
							break;
						}
					}
				}
				//Merge both blockmaps' block lists
				curBlockMap = CreateBlockMap();
				curBlockMap->m_base_allocation_size[0] = blockMaps[0]->m_base_allocation_size[0];
				curBlockMap->m_base_allocation_size[1] = blockMaps[1]->m_base_allocation_size[1];
				curBlockMap->m_virtual_len = blockMaps[0]->m_virtual_len;
				curBlockMap->m_physical_len = blockMaps[1]->m_physical_len;
				memcpy(&curBlockMap->m_blocks[0], &blockMaps[0]->m_blocks[0], curBlockMap->m_virtual_len * sizeof(BlockInfo));
				memcpy(&curBlockMap->m_blocks[curBlockMap->m_virtual_len], &blockMaps[1]->m_blocks[0], curBlockMap->m_physical_len * sizeof(BlockInfo));
				//Merge ptr lists
				curAllocatedPtrs.insert(curAllocatedPtrs.end(), ptrLists[0].begin(), ptrLists[0].end());
				curAllocatedPtrs.insert(curAllocatedPtrs.end(), ptrLists[1].begin(), ptrLists[1].end());
				//Delete the temp block maps
				g_allocationData.erase(blockMaps[0]);
				g_allocationData.erase(blockMaps[1]);
				delete blockMaps[0];
				delete blockMaps[1];
			}
			//Copy allocated data to its true new home
			//sortedAllocations.push_front(firstAlloc);
			std::vector<std::tuple<void*, size_t, bool>> fullAllocations(sortedAllocations.size() + 1);
			fullAllocations[0] = firstAlloc;
			std::copy(sortedAllocations.begin(), sortedAllocations.end(), fullAllocations.begin() + 1);
			sortedAllocations.clear();
			int i{};
			for (auto& alloc : fullAllocations) {
				char* startPtr{ reinterpret_cast<char*>(std::get<0>(alloc)) };
				char* endPtr{ startPtr + std::get<1>(alloc) };
				char* newStartPtr{ reinterpret_cast<char*>(curAllocatedPtrs[i]) };
				i++;
				memcpy(newStartPtr, startPtr, std::get<1>(alloc));
				auto begin{ g_packPointerSet->lower_bound((pgPtrRepresentation*)startPtr) };
				auto end{ g_packPointerSet->upper_bound((pgPtrRepresentation*)endPtr) };
				for (auto it{ begin }; it != end; ++it) {
					auto ptrLoc{ reinterpret_cast<char*>(*it) };
					ptrLoc = ptrLoc - startPtr + newStartPtr;
					auto rawPtr{ reinterpret_cast<char**>(ptrLoc) };
					auto ptr{ reinterpret_cast<pgPtrRepresentation*>(ptrLoc) };
					//Find the new allocation block that relates the most to this pointer
					int j{};
					for (auto& intAlloc : fullAllocations) {
						char* startPtr{ reinterpret_cast<char*>(std::get<0>(intAlloc)) };
						char* endPtr{ startPtr + std::get<1>(intAlloc) };
						char* newStartPtr{ reinterpret_cast<char*>(curAllocatedPtrs[j]) };
						j++;
						if (*rawPtr >= startPtr && *rawPtr < endPtr) {
							//Calculate the new relative pointer
							*rawPtr = (*rawPtr - startPtr + newStartPtr);
							//Find the allocation block this is supposed to be in
							for (int k{}; k != curBlockMap->m_physical_len + curBlockMap->m_virtual_len; ++k) {
								auto& block = curBlockMap->m_blocks[k];
								if (*rawPtr >= block.data && *rawPtr < ((char*)block.data + block.size)) {
									char* rawPtrValue = *rawPtr;
									ptr->m_block_type = (k >= curBlockMap->m_virtual_len) ? 6 : 5;
									ptr->m_pointer = (uintptr_t)((rawPtrValue - (char*)block.data) + block.offset);
									break;
								}
							}
							break;
						}
					}
				}
			}
			//Swap the block map?
			g_allocationData[blockMap] = std::move(g_allocationData[curBlockMap]);
			memcpy(blockMap, curBlockMap, sizeof(*curBlockMap));
			g_allocationData.erase(curBlockMap);
			delete curBlockMap;
		}
		static bool IsInBlockMap(void* ptr, BlockMap* blockMap, bool physical) {
			if (!blockMap) {
				blockMap = g_packBlockMap;
			}
			//Find an allocation block for this block map
			auto allocBlock = g_allocationData.find(blockMap);
			if (allocBlock == g_allocationData.end()) {
				return false;
			}
			auto& allocInfo = allocBlock->second;
			//A non-resolving block map can contain anything, so handle that
			if (!allocInfo.isPerformingFinalAllocation) {
				for (auto& allocation : allocInfo.allocations) {
					char* startPtr{ reinterpret_cast<char*>(std::get<0>(allocation)) };
					char* endPtr{ startPtr + std::get<1>(allocation) };
					if (ptr >= startPtr && ptr < endPtr) {
						return true;
					}
				}
				return false;
			}
			int startIndex{ physical ? blockMap->m_virtual_len : NULL };
			int endIndex{ physical ? blockMap->m_virtual_len + blockMap->m_physical_len : blockMap->m_virtual_len };
			uintptr_t inPtr{ uintptr_t(ptr) };
			for (int i{ startIndex }; i != endIndex; ++i) {
				auto& block = blockMap->m_blocks[i];
				auto offStart = (uintptr_t)block.data;
				auto offEnd = offStart + block.size;
				if (inPtr >= offStart && inPtr < offEnd) {
					return true;
				}
			}
			return false;
		}
		static void* Allocate(size_t size, bool isPhysical, BlockMap* blockMap) {
			const int8_t maxMults[] = { 16, 8, 4, 2, 1 };
			const uint8_t maxCounts[] = { 1, 3, 15, 63, 127 };
			//Is this the packing block map?
			void* oldBlockMap = blockMap;
			if (!blockMap || (uintptr_t)blockMap <= 0xF000) {
				blockMap = g_packBlockMap;
			}
			//Find an allocation block for this block map
			auto allocBlock = g_allocationData.find(blockMap);
			if (allocBlock == g_allocationData.end()) {
				return nullptr;
			}
			auto& allocInfo = allocBlock->second;
			//Are we performing pre-allocation?
			if (!allocInfo.isPerformingFinalAllocation) {
				void* retPtr = malloc(size);
				allocInfo.allocations.push_back(std::tuple<void*, size_t, bool>(retPtr, size, isPhysical));
				return retPtr;
			}
			//Start at the current offset thing
			int curBlock{ (isPhysical) ? (blockMap->m_physical_len + blockMap->m_virtual_len) : blockMap->m_virtual_len };
			if ((!isPhysical && blockMap->m_virtual_len > 0) || (isPhysical && blockMap->m_physical_len > 0)) {
				curBlock--;
			}
			auto& curBlockInfo = blockMap->m_blocks[curBlock];
			size = ((size % 16) == 0) ? size : (size + (16 - (size % 16)));
			//Make sure the allocation can't straddle a native page boundary (unless it's bigger than 0x200 bytes)
			uint32_t base = allocInfo.baseMemorySize;//0x4000; //Base * 16 is the largest allocation possible - TODO: provide a means for the user to set this/relocate bases (that'd cause all existing pages to be invalidated?)
			size_t allocOffset{};
			auto padAlloc = [&](size_t padSize) {
				char* pad = (char*)Allocate(padSize, false, (BlockMap*)0x8001);
				for (int i = 0; i < padSize; i++) {
					pad[i] = '1';
				}
				allocInfo.gapList[isPhysical].push_back(BlockMapGap{ pad, padSize });
				size_t curSize = curBlockInfo.offset + curBlockInfo.size;
				assert((curSize % base) == 0);
				return Allocate(size, isPhysical, (BlockMap*)0x8001);
			};
			if ((curBlockInfo.size + size) <= allocInfo.realMaxSizes[curBlock]) {
				//Try finding a gap to allocate ourselves into
				for (auto& gap : allocInfo.gapList[isPhysical]) {
					if (gap.size >= size) {
						void* ptr = gap.start;
						gap.size -= size;
						gap.start += size;
						return ptr;
					}
				}
				if ((curBlockInfo.size + size) > allocInfo.maxSizes[curBlock]) {
					allocInfo.maxSizes[curBlock] *= 2;
				}
				void* newPtr = (char*)curBlockInfo.data + curBlockInfo.size;
				curBlockInfo.size += size;
				memset(newPtr, 0xCD, size);
				*(int*)((char*)newPtr + size) = 0xDEADC0DE;
				return newPtr;
			}
			//Apparently we need to allocate a new block...
			//Pad the page to the maximum size so paging does not break
			if ((!isPhysical && blockMap->m_virtual_len > 0) || (isPhysical && blockMap->m_physical_len > 0))
			{
				char* pad = (char*)curBlockInfo.data + curBlockInfo.size;
				size_t padSize = allocInfo.realMaxSizes[curBlock] - curBlockInfo.size;
				memset(pad, '1', padSize);
				allocInfo.gapList[isPhysical].push_back(BlockMapGap{ pad, padSize });
				curBlockInfo.size += padSize;
			}
			//Move along all physical pages if we need a new virtual page
			if (!isPhysical && blockMap->m_physical_len > 0) {
				int moveStart{ blockMap->m_virtual_len };
				int moveEnd{ moveStart + blockMap->m_physical_len };
				for (int i{ moveEnd - 1 }; i >= moveStart; --i) {
					if ((i + 1) >= _countof(blockMap->m_blocks)) {
						LOG(Fatal, "Ran out of possible blocks");
					}
					blockMap->m_blocks[i + 1] = blockMap->m_blocks[i];
				}
			}
			//Determine the new offset
			int newOffset{};
			size_t newSize = base; //64 * 1024;
			if (isPhysical) {
				int lastIdx{};
				//Find the last physical block
				if (blockMap->m_physical_len > 0) {
					lastIdx = blockMap->m_virtual_len + blockMap->m_physical_len - 1;
					auto& lastBlock = blockMap->m_blocks[lastIdx];
					newOffset = lastBlock.offset + lastBlock.size;
				}
				else {
					newOffset = 0;
				}
			}
			else {
				int lastIdx{};
				//Find the last physical block
				if (blockMap->m_virtual_len > 0) {
					lastIdx = blockMap->m_virtual_len - 1;\
					auto& lastBlock = blockMap->m_blocks[lastIdx];
					newOffset = lastBlock.offset + lastBlock.size;
				}
				else {
					newOffset = 0;
				}
			}
			while (size >= newSize) {
				newSize *= 2;
			}
			//Determine the new block index
			int newStart = (!isPhysical) ? blockMap->m_virtual_len : (blockMap->m_virtual_len + blockMap->m_physical_len);
			auto& newBlockInfo = blockMap->m_blocks[newStart];
			allocInfo.maxSizes[newStart] = newSize;
			//Calculate the real maximum size
			int curMult{};
			int curCount{};
			for (int i = (isPhysical ? blockMap->m_virtual_len : 0); i < blockMap->m_virtual_len + (isPhysical ? blockMap->m_physical_len : 0); i++) {
				curCount++;
				if (curCount == maxCounts[curMult]) {
					curMult++;
					curCount = 0;
				}
			}
			curMult = maxMults[curMult];
			size_t multipliedBase = (curMult >= 0) ? (base * curMult) : (base / -curMult);
			if (newSize > multipliedBase) {
				return nullptr;
			}
			newBlockInfo.data = malloc(multipliedBase + 4);
			memset(newBlockInfo.data, 0xCD, multipliedBase);
			allocInfo.realMaxSizes[newStart] = multipliedBase;
			newBlockInfo.offset = newOffset;
			newBlockInfo.size = size;
			*(int*)((char*)newBlockInfo.data + size) = 0xDEADC0DE;
			if (isPhysical) {
				blockMap->m_physical_len++;
			}
			else {
				blockMap->m_virtual_len++;
			}
			return newBlockInfo.data;
		}
		static void* AllocatePlacement(size_t size, void* hintPtr, bool isPhysical, BlockMap* blockMap) {
			if (IsInBlockMap(hintPtr, blockMap, isPhysical)) {
				return hintPtr;
			}
			return Allocate(size, isPhysical, blockMap);
		}
		static BlockMap* GetBlockMap() {
			return g_packBlockMap;
		}
		static inline char* StringDup(const char* str) {
			char* outStr = (char*)Allocate(strlen(str) + 1, false, nullptr);
			strcpy(outStr, str);
			return outStr;
		}
	};
}