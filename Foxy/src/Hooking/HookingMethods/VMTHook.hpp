#pragma once
#include <windows.h>
#include <memory>

namespace base {
	class hookVFT {
	public:
		hookVFT(void* object, size_t numFuncs) :
			m_obj(reinterpret_cast<uintptr_t**>(object)),
			m_numFuncs(numFuncs),
			m_og(*m_obj),
			m_new(std::make_unique<uintptr_t[]>(m_numFuncs + 1))
		{
			std::copy_n(m_og - 1, m_numFuncs + 1, m_new.get());
		}
		~hookVFT() = default;
		void enable() {
			*m_obj = &m_new[1];
		}
		void disable() {
			*m_obj = m_og;
		}
		void hook(void* replacement, size_t index) {
			m_new[index + 1] = (uintptr_t)replacement;
		}
		void unhook(size_t index) {
			m_new[index + 1] = m_og[index];
		}
		template <typename T>
		T getOg(size_t index) {
			return (T)m_og[index];
		}
	private:
		uintptr_t** m_obj;
		size_t m_numFuncs;
		uintptr_t* m_og;
		std::unique_ptr<uintptr_t[]> m_new;
	};
}