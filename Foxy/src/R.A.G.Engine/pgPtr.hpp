#pragma once
#include "pgStreamableBase.hpp"

namespace rage {
	template<typename T, bool Physical = false>
	class pgPtr : public pgStreamableBase {
	private:
		union {
			pgPtrRepresentation on_disk;
			T* pointer;
		};
	public:
		pgPtr() {
			pointer = 0;
		}
		~pgPtr() {
			pgStreamManager::UnmarkResolved(this);
		}
		T* operator->() const {
			if (pgStreamManager::IsResolved(this))
			{
				return (T*)pointer;
			}
			else
			{
				return (T*)pgStreamManager::ResolveFilePointer(const_cast<pgPtrRepresentation&>(on_disk));
			}
		}
		T* operator*() const {
			if (pgStreamManager::IsResolved(this)) {
				return (T*)pointer;
			}
			else {
				return (T*)pgStreamManager::ResolveFilePointer(const_cast<pgPtrRepresentation&>(on_disk));
			}
		}
		pgPtr operator=(T* other) {
			pointer = (decltype(pointer))other;
			return *this;
		}
		pgPtr(pgPtrRepresentation fromRep) {
			on_disk = fromRep;
		}
		pgPtr(const pgPtr& from) {
			on_disk = from.on_disk;
		}
		pgPtr& operator=(const pgPtr& arg) {
			pointer = arg.pointer;
			return *this;
		}
		bool IsNull() {
			return (pointer == 0);
		}
		void Resolve(BlockMap* blockMap = nullptr) {
			if (on_disk.blockType == 0 && on_disk.pointer == 0) {
				return;
			}
			if (!pgStreamManager::IsResolved(this)) {
				bool physical = (on_disk.blockType == 6);
				pointer = (T*)pgStreamManager::ResolveFilePointer(on_disk, blockMap);
				pgStreamManager::MarkToBePacked(&on_disk, physical, "ResolveFilePointer");
				pgStreamManager::MarkResolved(this);
			}
		}
	};
}