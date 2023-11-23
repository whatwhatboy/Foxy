#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Natives/Invoker/Invoker.hpp"
#include <R.A.G.Engine/Natives/Crossmap.hpp>

namespace base {
	class yscFile {
	public:
		yscFile(fs::path filename) {
			m_file.open(filename.string(), std::ios::in | std::ios::binary);
		}
		~yscFile() {
			for (auto& code : m_codeBlockList)
				delete[] code;
			for (auto& string : m_stringBlockList)
				delete[] string;
			if (m_statics)
				delete[] m_statics;
		}
	public:
		void load() {
			m_file.seekg(0, std::ios_base::beg);
			m_file.seekg(8, std::ios_base::cur); // page base
			m_file.seekg(8, std::ios_base::cur); // unk 1 ptr
			m_file.read((char*)&m_codePageListOffset, 8); fixPointer(m_codePageListOffset);
			m_file.seekg(4, std::ios_base::cur); // unk 2
			m_file.read((char*)&m_codeLength, 4);
			m_file.read((char*)&m_scriptParameterCount, 4);
			m_file.read((char*)&m_staticCount, 4);
			m_file.read((char*)&m_globalCount, 4);
			m_file.read((char*)&m_nativesCount, 4);
			m_file.read((char*)&m_staticsOffset, 8); fixPointer(m_staticsOffset);
			m_file.seekg(8, std::ios_base::cur); // globals offset
			m_file.read((char*)&m_nativesListOffset, 8); fixPointer(m_nativesListOffset);
			m_file.seekg(8, std::ios_base::cur); // unk 3 ptr
			m_file.seekg(8, std::ios_base::cur); // unk 4 ptr
			m_file.read((char*)&m_nameHash, 4);
			m_file.seekg(4, std::ios_base::cur); // unk 5
			m_file.read((char*)&m_scriptNameOffset, 8); fixPointer(m_scriptNameOffset);
			m_file.read((char*)&m_stringsListOffset, 8); fixPointer(m_stringsListOffset);
			m_file.read((char*)&m_stringSize, 4);
			m_stringBlocks = (m_stringSize + 0x3FFF) >> 14;
			m_codeBlocks = (m_codeLength + 0x3FFF) >> 14;
			m_file.seekg(m_scriptNameOffset, std::ios_base::beg);
			char chr = 0;
			uint32_t i = 0;
			while (i < 0x40) {
				m_file.read((char*)&chr, 1);
				m_name[i] = chr;
				if (chr == 0x0 || chr == 0xFF)
					break;
			}
			if (m_staticCount) {
				m_file.seekg(m_staticCount, std::ios_base::beg);
				m_statics = new uint64_t[m_staticCount];
				m_file.read((char*)m_statics, m_staticCount * 8);
			}
			for (i = 0; i < m_codeBlocks; ++i) {
				uint64_t loc;
				uint32_t tablesize = ((i + 1) * 0x4000u >= m_codeLength) ? m_codeLength % 0x4000u : 0x4000u;
				uint8_t* code_block = new uint8_t[tablesize];
				m_file.seekg(m_codePageListOffset + (i * 8), std::ios_base::beg);
				m_file.read((char*)&loc, 8); fixPointer(loc);
				m_file.seekg(loc, std::ios_base::beg);
				m_file.read((char*)code_block, tablesize);
				m_codeBlockList.push_back(code_block);
			}
			for (i = 0; i < m_stringBlocks; ++i) {
				uint64_t loc;
				int tablesize = ((i + 1) * 0x4000 >= m_stringSize) ? m_stringSize % 0x4000u : 0x4000u;
				uint8_t* string_block = new uint8_t[tablesize];
				m_file.seekg(m_stringsListOffset + (i * 8), std::ios_base::beg);
				m_file.read((char*)&loc, 8); fixPointer(loc);
				m_file.seekg(loc, std::ios_base::beg);
				m_file.read((char*)string_block, tablesize);
				m_stringBlockList.push_back(string_block);
			}
			for (i = 0; i != m_nativesCount; ++i) {
				m_file.seekg(m_nativesListOffset + (i * 8), std::ios_base::beg);
				uint64_t hash{};
				m_file.read((char*)&hash, 8); rotl(hash, m_codeLength + i);
				m_natives.push_back(g_invoker.m_cache[hash]);
			}
			m_file.close();
		}
	public:
		void fixPointer(std::uint64_t& ptr) {
			ptr &= 0xFFFFFF;
		}
		void rotl(std::uint64_t& hash, int rotate) {
			rotate %= 64;
			hash = (hash << rotate | hash >> (64 - rotate));
		}
	public:
		std::ifstream m_file;
	public:
		std::vector<uint8_t*> m_codeBlockList{};
		std::vector<uint8_t*> m_stringBlockList{};
		std::vector<rage::Cmd> m_natives{};
		uint32_t m_nameHash;
		char m_name[0x40];
		uint64_t* m_statics = nullptr;
		uint64_t m_codePageListOffset;
		uint64_t m_stringsListOffset;
		uint64_t m_staticsOffset;
		uint64_t m_nativesListOffset;
		uint64_t m_scriptNameOffset;
		uint32_t m_codeLength;
		uint32_t m_scriptParameterCount;
		uint32_t m_staticCount;
		uint32_t m_globalCount;
		uint32_t m_nativesCount;
		uint32_t m_stringSize;
		uint32_t m_codeBlocks;
		uint32_t m_stringBlocks;
	};
}