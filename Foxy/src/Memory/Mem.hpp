#pragma once
#include "Pch/Common.hpp"

namespace base::memory {
	class mem {
	public:
		template <typename ptr_t = void*>
		mem* setPtr(ptr_t p) {
			m_ptr = (void*)p;
			return this;
		}
		mem(void* p = nullptr) { setPtr(p); }
		mem(uintptr_t p) { setPtr(p); }
		template <typename t> std::enable_if_t<std::is_pointer_v<t>, t> as() { return static_cast<t>(m_ptr); }
		template <typename t> std::enable_if_t<std::is_lvalue_reference_v<t>, t> as() { return *static_cast<std::add_pointer_t<std::remove_reference_t<t>>>(m_ptr); }
		template <typename t> std::enable_if_t<std::is_same_v<t, uintptr_t>, t> as() { return t(m_ptr); }
		mem add(size_t offset) { return mem(as<uintptr_t>() + offset); }
		mem sub(size_t offset) { return mem(as<uintptr_t>() - offset); }
		mem rip() { return add(as<int32_t&>()).add(4); }
		operator bool() noexcept { return m_ptr; }
		void* m_ptr{};
	};
	template <typename returnType>
	inline returnType readRelative(char* addr, int instructionSize = 0) {
		auto instructionLen = 4 + instructionSize;
		return returnType(*(int32_t*)(addr + instructionSize) + addr + instructionLen);
	}
	template <typename returnType>
	inline returnType readInstruction(char* addr, int instructionSize, int instructionLen) {
		return returnType(*(int32_t*)(addr + instructionSize) + addr + instructionLen);
	}
}