#pragma once
#include "Pch/Common.hpp"
#include "Core/Logger.hpp"
#include "Mem.hpp"
#include "Module.hpp"

namespace base::memory::util {
	inline bool doesMemoryMatch(uint8_t* target, std::optional<uint8_t> const* sig, size_t len) {
		for (size_t i{}; i != len; ++i)
			if (sig[i] && *sig[i] != target[i])
				return false;
		return true;
	}
	inline uint64_t findPatternBruteforce(std::vector<std::optional<uint8_t>> bytes, hmod mod = {}) {
		for (uintptr_t i{}; i != mod.m_size - bytes.size(); ++i)
			if (doesMemoryMatch(mod.m_begin.add(i).as<uint8_t*>(), bytes.data(), bytes.size()))
				return mod.m_begin.as<uint64_t>() + i;
		return NULL;
	}
	inline uint64_t findPatternBoyerMooreHorspool(std::vector<std::optional<uint8_t>> bytes, hmod module = {}) {
		size_t maxShift{ bytes.size() };
		size_t maxIdx{ maxShift - 1 };
		//Get wildcard index, and store max shifable byte count
		size_t wildCardIdx{ size_t(-1) };
		for (int i{ int(maxIdx - 1) }; i >= 0; --i) {
			if (!bytes[i]) {
				maxShift = maxIdx - i;
				wildCardIdx = i;
				break;
			}
		}
		//Store max shiftable bytes for non wildcards.
		size_t shiftTable[UINT8_MAX + 1]{};
		for (size_t i{}; i <= UINT8_MAX; ++i)
			shiftTable[i] = maxShift;
		for (size_t i{ wildCardIdx + 1 }; i != maxIdx; ++i)
			shiftTable[*bytes[i]] = maxIdx - i;
		//Loop data
		for (size_t curIdx{}; curIdx != module.m_size - bytes.size();) {
			for (size_t sigIdx = maxIdx; sigIdx >= 0; --sigIdx) {
				if (bytes[sigIdx] && *module.m_begin.add(curIdx + sigIdx).as<uint8_t*>() != *bytes[sigIdx]) {
					curIdx += shiftTable[*module.m_begin.add(curIdx + maxIdx).as<uint8_t*>()];
					break;
				}
				else if (sigIdx == NULL) {
					return module.m_begin.add(curIdx).as<uint64_t>();
				}
			}
		}
		return NULL;
	}
	inline uint32_t g_foundSigCount{}, g_failedSigCount{}, g_totalSigCount{};
	inline std::optional<uint8_t> charToHex(char const c) {
		switch (c) {
		case '0':
			return static_cast<uint8_t>(0x0);
		case '1':
			return static_cast<uint8_t>(0x1);
		case '2':
			return static_cast<uint8_t>(0x2);
		case '3':
			return static_cast<uint8_t>(0x3);
		case '4':
			return static_cast<uint8_t>(0x4);
		case '5':
			return static_cast<uint8_t>(0x5);
		case '6':
			return static_cast<uint8_t>(0x6);
		case '7':
			return static_cast<uint8_t>(0x7);
		case '8':
			return static_cast<uint8_t>(0x8);
		case '9':
			return static_cast<uint8_t>(0x9);
		case 'A':
			return static_cast<uint8_t>(0xA);
		case 'B':
			return static_cast<uint8_t>(0xB);
		case 'C':
			return static_cast<uint8_t>(0xC);
		case 'D':
			return static_cast<uint8_t>(0xD);
		case 'E':
			return static_cast<uint8_t>(0xE);
		case 'F':
			return static_cast<uint8_t>(0xF);
		}
		return std::nullopt;
	}
	inline std::vector<std::optional<uint8_t>> createBytesFromString(std::string ptr) {
		std::vector<std::optional<uint8_t>> bytes{};
		for (size_t i{}; i != ptr.size(); ++i) {
			if (ptr[i] == ' ')
				continue;
			if (ptr[i] != '?') {
				if (i != ptr.size() - 1) {
					auto c1 = charToHex(ptr[i]);
					auto c2 = charToHex(ptr[i + 1]);
					if (c1 && c2)
						bytes.emplace_back(static_cast<uint8_t>((*c1 * 0x10) + *c2));
				}
			}
			else {
				bytes.push_back({});
			}
		}
		return bytes;
	}
}