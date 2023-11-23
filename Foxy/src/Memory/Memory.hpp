#pragma once
#include "Pch/Common.hpp"
#include "Module.hpp"
#include "Mem.hpp"
#include "Memory/Signature.hpp"

namespace base::memory {
	class memory {
	#define RIP_INSTRUCTION static_cast<int8_t>('.')
	#define MOV_INSTRUCTION static_cast<int8_t>(0xB8)
	#define AND_INSTRUCTION static_cast<int8_t>(0x25)
	#define LEA_INSTRUCTION static_cast<int8_t>(0x8D)
	#define CMP_INSTRUCTION static_cast<int8_t>(0x80)
	#define ADD_INSTRUCTION static_cast<int8_t>(0x81)
	#define CALL_INSTRUCTION static_cast<int8_t>(0xE8)
	public:
		memory(hmod module, std::string value, char const* name, char const* sig) : m_value(value), m_name(name), m_signature(sig), m_module(module) {}
		memory() = default;
	public:
		bool parseInstructions() {
			static auto splitString = [](std::string str, std::int8_t split) -> std::vector<std::string> {
				std::vector<std::string> fields{};
				std::string field{};
				for (std::int32_t i{}; i != str.length(); ++i) {
					if (str.at(i) == split) {
						fields.push_back(field);
						field.erase();
					}
					else {
						field += str.at(i);
						if (i == str.length() - 1)
							fields.push_back(field);
					}
				}
				return fields;
			};
			auto handleField = [&](std::string f) {
				if (f.find("rip") != -1) {
					m_instructions.push_back(RIP_INSTRUCTION);
				}
				else if (f.find("mov") != -1) {
					m_instructions.push_back(MOV_INSTRUCTION);
				}
				else if (f.find("and") != -1) {
					m_instructions.push_back(AND_INSTRUCTION);
				}
				else if (f.find("lea") != -1) {
					m_instructions.push_back(LEA_INSTRUCTION);
				}
				else if (f.find("cmp") != -1) {
					m_instructions.push_back(CMP_INSTRUCTION);
				}
				else if (f.find("add") != -1) {
					m_instructions.push_back(ADD_INSTRUCTION);
				}
				else if (f.find("call") != -1) {
					m_instructions.push_back(CALL_INSTRUCTION);
				}
				else {
					bool isSymbolPresent{ f.find("+") != -1 || f.find("-") != -1 };
					auto insEndOffset = f.find(",") != -1 ? 1 : 0;
					std::string ins{ f.substr(1, f.size() - insEndOffset) };
					if (ins.find(",") != -1) {
						insEndOffset++;
						ins = f.substr(1, f.size() - insEndOffset);
					}
					if (!isSymbolPresent) {
						ins = f.substr(0, f.size() - insEndOffset);
					}
					if (!containsANumber(ins))
						return false;
					int8_t insValue{};
					if (containsANumber(ins)) {
						if (isNumber(ins))
							insValue = static_cast<int8_t>(std::stoi(ins));
						else
							insValue = static_cast<int8_t>(std::strtoull(ins.c_str(), 0, 16));
					}
					int8_t negInsValue{ 0 - insValue };
					bool isNegative{};
					switch (f[0]) {
					case '-': {
						isNegative = true;
						m_instructions.push_back(negInsValue);
					} break;
					case '+': {
						m_instructions.push_back(insValue);
					} break;
					default: {
						m_instructions.push_back(insValue);
					} break;
					}
				}
			};
			if (m_value.empty()) {
				return false;
			}
			auto fields{ splitString(m_value, ' ') };
			if (!fields.size()) {
				handleField(m_value);
			}
			else {
				for (auto& f : fields) {
					handleField(f);
				}
			}
			return true;
		}
		bool parse() {
			if (!m_value.empty()) {
				if (m_instructions.empty() || !m_instructions.size()) {
					if (!parseInstructions())
						return false;
				}
			}
			if (!m_pointer) {
				util::g_totalSigCount++;
				m_pointer = scan();
				if (!m_pointer) {
					util::g_failedSigCount++;
					return false;
				}
				util::g_foundSigCount++;
			}
			m_pointerCopy = m_pointer;
			if (!m_instructions.empty()) {
				for (std::size_t i{}; i != m_instructions.size(); ++i) {
					switch (m_instructions[i]) {
					case RIP_INSTRUCTION: {
						m_pointer = readRelative<uint64_t>((char*)m_pointer);
					} break;
					case MOV_INSTRUCTION: {
						m_pointer = readInstruction<uint64_t>((char*)m_pointer, 3, 7);
					} break;
					case AND_INSTRUCTION: {
						m_pointer = readInstruction<uint64_t>((char*)m_pointer, 2, 6);
					} break;
					case LEA_INSTRUCTION: {
						m_pointer = readInstruction<uint64_t>((char*)m_pointer, 2, 6);
					} break;
					case CMP_INSTRUCTION: {
						m_pointer = readInstruction<uint64_t>((char*)m_pointer, 3, 7);
					} break;
					case ADD_INSTRUCTION: {
						m_pointer = readInstruction<uint64_t>((char*)m_pointer, 3, 7);
					} break;
					case CALL_INSTRUCTION: {
						m_pointer = readInstruction<uint64_t>((char*)m_pointer, 1, 5);
					} break;
					default: {
						m_pointer += m_instructions[i];
					} break;
					}
				}
			}
			return true;
		}
	public:
		uint64_t scan() {
			return util::findPatternBoyerMooreHorspool(util::createBytesFromString(m_signature), m_module);
		}
	public:
		template <typename t>
		t get(bool raw) {
			return raw ? (t)m_pointerCopy : (t)m_pointer;
		}
		uint64_t operator*() {
			return get<uint64_t>(false);
		}
	public:
		std::string m_value{};
		char const* m_name{};
		char const* m_signature{};
		hmod m_module{};
	private:
		uint64_t m_pointer{};
		uint64_t m_pointerCopy{};
		std::vector<int8_t> m_instructions{};
	};
}