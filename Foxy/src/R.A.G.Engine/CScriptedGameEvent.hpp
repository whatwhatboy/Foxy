#pragma once
#include <cstdint>
#include "NetGameEvent.hpp"

class CScriptedGameEvent : public rage::netGameEvent {
public:
	char pad_00030[64]; //0x0030
	int64_t m_args[54]; //0x0070
	uint32_t m_bitset; //0x0220
	uint32_t m_args_size; //0x0224
}; //Size: 0x0228