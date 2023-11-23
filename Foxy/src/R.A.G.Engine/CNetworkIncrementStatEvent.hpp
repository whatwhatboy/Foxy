#pragma once
#include <cstdint>
#include "NetGameEvent.hpp"

class CNetworkIncrementStatEvent : public rage::netGameEvent {
public:
	uint32_t m_stat; //0x0030
	uint32_t m_amount; //0x0034
}; //Size: 0x0038