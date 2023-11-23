#pragma once
#include <cstdint>

namespace rage {
	template <typename T>
	class atArray {
	public:
		T* begin() { return m_data; }
		T* end() { return m_data + m_size; }
		const T* begin() const { return m_data; }
		const T* end() const { return m_data + m_size; }
		T* data() { return m_data; }
		const T* data() const { return m_data; }
		uint16_t size() const { return m_size; }
		uint16_t capacity() const { return m_capacity; }
		T& operator[](uint16_t index) { return m_data[index]; }
		const T& operator[](uint16_t index) const { return m_data[index]; }
	private:
		T* m_data; //0x0000
		uint16_t m_size; //0x0008
		uint16_t m_capacity; //0x0010
	}; //Size: 0x0010
	static_assert(sizeof(rage::atArray<void*>) == 0x10);
	template <typename T, uint16_t arraySize>
	class atFixedArray {
	public:
		T* begin() { return m_data; }
		T* end() { return m_data + m_size; }
		const T* begin() const { return m_data; }
		const T* end() const { return m_data + m_size; }
		T* data() { return m_data; }
		const T* data() const { return m_data; }
		uint16_t size() const { return m_size; }
		uint16_t capacity() const { return m_capacity; }
		T& operator[](uint16_t index) { return m_data[index]; }
		const T& operator[](uint16_t index) const { return m_data[index]; }
	private:
		T* m_data; //0x0000
		uint16_t m_size = arraySize; //0x0008
		uint16_t m_capacity = arraySize; //0x0010
	};
}