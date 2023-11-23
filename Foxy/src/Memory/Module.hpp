#pragma once
#include "Pch/Common.hpp"
#include "Mem.hpp"
#define DEFAULT_MODULE "GTA5.exe"

namespace base::memory {
	class hmod {
	public:
		hmod(HMODULE hmod, char const* moduleName) : m_moduleName(moduleName) {
			m_begin.setPtr(hmod);
			auto dosHdr = reinterpret_cast<IMAGE_DOS_HEADER*>(m_begin.as<char*>());
			auto ntHdr = reinterpret_cast<IMAGE_NT_HEADERS64*>(m_begin.as<char*>() + dosHdr->e_lfanew);
			m_size = ntHdr->OptionalHeader.SizeOfImage;
			m_end = m_begin.as<char*>() + m_size;
		}
		hmod(char const* hmod) : hmod(GetModuleHandleA(hmod), hmod) {}
		hmod(std::string_view hmod) : hmod(hmod.data()) {}
		hmod() : hmod(GetModuleHandleA(NULL), DEFAULT_MODULE) {}
	public:
		mem m_begin{}, m_end{};
		size_t m_size{};
		std::string_view m_moduleName{};
	};
}