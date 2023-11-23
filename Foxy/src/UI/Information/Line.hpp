#pragma once
#include "Pch/Common.hpp"
#include "UI/Manager/DXUiManager.hpp"

namespace base::ui::informational {
	struct line {
		line(std::string l, std::string r) : m_left(l), m_right(r) {}
		virtual ~line() = default;
		virtual void draw(dxUiManager* mgr, ImVec2 pos, float offset, float& drawBase) {
			ImFont* font{ g_renderer->m_font };
			ImVec2 m_pos{ pos.x + offset, drawBase };
			mgr->rect(ImVec2(m_pos.x, m_pos.y + m_size.y / 2.f), m_size, m_bgColor);
			if (!m_left.empty())
				mgr->leftText(m_left, ImVec2(m_pos.x - (m_size.x / 2.1f), m_pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_left).y / 2.f)), m_txtColor, font);
			if (!m_right.empty())
				mgr->rightText(m_right, ImVec2(m_pos.x + (m_size.x / 2.1f), m_pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_right).y / 2.f)), m_txtColor, font);
			drawBase += m_size.y;
		}

		std::string m_left{};
		std::string m_right{};
		ImU32 m_bgColor{ IM_COL32(10, 10, 10, 205) };
		ImU32 m_txtColor{ IM_COL32(255, 255, 255, 255) };
		ImVec2 m_size{ 0.22f, 0.026f };
	};
}