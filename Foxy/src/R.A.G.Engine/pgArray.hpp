#pragma once
#include "pgStreamableBase.hpp"

namespace rage {
	template <typename TValue, typename TIndex = uint16_t>
	class pgArray : public pgStreamableBase {
	public:
		pgArray() {
			m_offset = (TValue*)0;
			m_count = 0;
			m_size = 0;
		}
		pgArray(int capacity) {
			m_offset = (TValue*)rage::sysMemAllocator::get()->Allocate(sizeof(TValue) * capacity, 16, 0);
			m_count = 0;
			m_size = capacity;
		}
		pgArray(TValue* values, int count) {
			SetFrom(values, count);
		}
		void SetFrom(TValue* values, int count) {
			m_offset = (TValue*)pgStreamManager::Allocate(count * sizeof(TValue), false, nullptr);
			std::copy(values, values + count, *m_offset);

			m_count = count;
			m_size = count;
		}
		TValue& Get(TIndex offset) {
			return (*m_offset)[offset];
		}
		auto& operator[](TIndex offset) {
			return Get(offset);
		}
		void Expand(TIndex newSize) {
			if (m_size >= newSize) {
				return;
			}
			auto newOffset = (TValue*)rage::sysMemAllocator::get()->Allocate(sizeof(TValue) * newSize, 16, 0);
			std::copy((*m_offset), (*m_offset) + m_count, newOffset);
			rage::sysMemAllocator::get()->Free(*m_offset);
			m_offset = newOffset;
			m_size = newSize;
		}
		pgArray* MakeSaveable() {
			return new(false) pgArray(*m_offset, m_count);
		}
		void Set(TIndex offset, const TValue& value) {
			if (offset >= m_size) {
				Expand(offset + 1);
			}
			if (offset >= m_count) {
				m_count = offset + 1;
			}
			(*m_offset)[offset] = value;
		}
		TIndex GetSize() const {
			return m_size;
		}
		TIndex GetCount() const {
			return m_count;
		}
		void Resolve(BlockMap* blockMap = nullptr) {
			m_offset.Resolve(blockMap);
		}
	private:
		pgPtr<TValue> m_offset;
		TIndex m_count;
		TIndex m_size;
	};
}