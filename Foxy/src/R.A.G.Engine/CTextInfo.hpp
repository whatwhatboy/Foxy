#pragma once
#include "Colors.hpp"

class CTextInfo {
public:
	RGBA m_color;
	float m_text_scale[2];
	float m_text_warp[2];
	int64_t	m_font;
	uint16_t m_not_centered;
	uint8_t m_drop_shadow;
	uint8_t m_text_outline;
public:
	void set_color(RGBA value) {
        value.swapValues(eColorValue::r, eColorValue::b);
		m_color	= value;
	}
	void set_scale(float value) {
		m_text_scale[0]	= value;
		m_text_scale[1]	= value;
	}
};