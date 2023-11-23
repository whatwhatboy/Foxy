#pragma once
#include "Pch/Common.hpp"
#include "Mem.hpp"
#include "Module.hpp"
#include "Util.hpp"

namespace base::memory {
	class signature {
	public:
		signature(char const* sig) {
			std::string ptr{ sig };
			for (size_t i{}; i != ptr.size(); ++i) {
				if (ptr[i] == ' ')
					continue;
				if (ptr[i] != '?') {
					if (i != ptr.size() - 1) {
						auto c1 = util::charToHex(ptr[i]);
						auto c2 = util::charToHex(ptr[i + 1]);
						if (c1 && c2)
							m_bytes.emplace_back(static_cast<uint8_t>((*c1 * 0x10) + *c2));
					}
				}
				else {
					m_bytes.push_back({});
				}
			}
		}
		uint64_t get(hmod mod = {}) {
			return util::findPatternBoyerMooreHorspool(m_bytes, mod);
		}
		uint64_t getLegacy(hmod mod = {}) {
			return util::findPatternBruteforce(m_bytes, mod);
		}
		std::vector<uint64_t> getAll(hmod mod = {}) {
			std::vector<uint64_t> ptrs{};
			for (uintptr_t i = 0; i < mod.m_size - m_bytes.size(); ++i)
				if (util::doesMemoryMatch(mod.m_begin.add(i).as<uint8_t*>(), m_bytes.data(), m_bytes.size()))
					ptrs.push_back(mod.m_begin.add(i).as<uint64_t>());
			return ptrs;
		}
	public:
		std::vector<std::optional<uint8_t>> m_bytes{};
	};
}