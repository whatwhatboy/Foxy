#pragma once

class CEventNetworkTextMessageReceived {
public:
	char pad_0000[24]; //0x0000
	char m_info[44]; //0x0018
}; //Size: 0x0044
static_assert(sizeof(CEventNetworkTextMessageReceived) == 0x44);