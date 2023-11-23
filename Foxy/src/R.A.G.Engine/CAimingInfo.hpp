#pragma once
#include <cstdint>

class CAimingInfo {
public:
	uint32_t m_name; //0x0000
	float m_heading_limit; //0x0004
	float m_sweep_pitch_min; //0x0008
	float m_sweep_pitch_max; //0x000C
}; //Size: 0x0010
static_assert(sizeof(CAimingInfo) == 0x10);