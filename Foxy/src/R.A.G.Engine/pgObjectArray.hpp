#pragma once
#include "pgStreamableBase.hpp"
#include "pgPtr.hpp"

namespace rage {
	#pragma pack(push, 1)
	template<typename TValue>
	class pgObjectArray : public pgStreamableBase {
	public:
		pgObjectArray() {
			m_objects = (pgPtr<TValue>*)0;
			m_count = 0;
			m_size = 0;
		}
		pgObjectArray(int capacity) {
			m_objects = (pgPtr<TValue>*)rage::sysMemAllocator::get()->Allocate(sizeof(pgPtr<TValue>) * capacity, 16, 0);
			m_count = 0;
			m_size = capacity;
		}
		pgObjectArray(pgPtr<TValue>* objects, int count) {
			SetFrom(objects, count);
		}
		void SetFrom(pgPtr<TValue>* objects, int count) {
			m_objects = (pgPtr<TValue>*)pgStreamManager::Allocate(count * sizeof(pgPtr<TValue>), false, nullptr);
			for (int i{}; i != count; ++i) {
				auto object = *(objects[i]);
				if (!object) {
					(*m_objects)[i] = nullptr;
					continue;
				}
				if (pgStreamManager::IsInBlockMap(object, nullptr, false)) {
					(*m_objects)[i] = object;
				}
				else {
					(*m_objects)[i] = new(pgStreamManager::Allocate(sizeof(*object), false, nullptr)) TValue(*object);
				}
			}
			m_count = count;
			m_size = count;
		}
		void Expand(uint16_t newSize) {
			if (m_size >= newSize) {
				return;
			}
			auto newObjects = (pgPtr<TValue>*)rage::sysMemAllocator::get()->Allocate(sizeof(pgPtr<TValue>) * newSize, 16, 0);
			std::copy((*m_objects), (*m_objects) + m_count, newObjects);
			rage::sysMemAllocator::get()->Free(*m_objects);
			m_objects = newObjects;
			m_size = newSize;
		}
		TValue* Get(uint16_t offset) {
			if (offset >= m_count) {
				return nullptr;
			}
			return *((*m_objects)[offset]);
		}
		auto& operator[](uint16_t offset) {
			return ((*m_objects)[offset]);
		}
		pgObjectArray* MakeSaveable() {
			return new(false) pgObjectArray(*m_objects, m_count);
		}
		void Set(uint16_t offset, TValue* value) {
			if (offset >= m_size) {
				Expand(offset + 1);
			}
			if (offset >= m_count) {
				m_count = offset + 1;
			}
			(*m_objects)[offset] = value;
		}
		uint16_t GetSize() const {
			return m_size;
		}
		uint16_t GetCount() const {
			return m_count;
		}
		inline void Resolve(BlockMap* blockMap = nullptr) {
			m_objects.Resolve(blockMap);
			for (int i{}; i != m_size; ++i) {
				(*m_objects)[i].Resolve(blockMap);
				if (!(*m_objects)[i].IsNull()) {
					(*m_objects)[i]->Resolve(blockMap);
				}
			}
		}
	private:
		pgPtr<pgPtr<TValue>> m_objects; //0x0000
		uint16_t m_count; //0x0008
		uint16_t m_size; //0x000C
		char pad_0000[4]; //0x0010
	}; //Size: 0x0014
	#pragma pack(pop)
}