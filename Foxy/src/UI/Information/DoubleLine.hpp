#pragma once
#include "UI/Information/Line.hpp"
#include "UI/Information/FormattedString.hpp"

namespace base::ui::informational {
	struct doubleLine : public line {
		doubleLine(formattedString str, formattedString str2) : m_str(str), m_str2(str2), m_strSplit(str.midSplit(':')), m_str2Split(str2.midSplit(':')), line(m_strSplit.first, m_str2Split.second) {}
		void draw(dxUiManager* mgr, ImVec2 pos, float offset, float& drawBase) override {
			ImVec2 m_pos{ pos.x + offset, drawBase };
			ImFont* font{ g_renderer->m_font };
			line::draw(mgr, pos, offset, drawBase);
			mgr->rightText(m_strSplit.second, ImVec2(m_pos.x - m_lineSize.x - 0.001f, m_pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_strSplit.second).y / 2.f)), m_txtColor, font);
			mgr->rect(ImVec2(m_pos.x, m_pos.y + m_size.y / 2.f), m_lineSize, m_lineColor);
			mgr->leftText(m_str2Split.first, ImVec2(m_pos.x + m_lineSize.x + 0.001f, m_pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_str2Split.first).y / 2.f)), m_txtColor, font);
		}
		formattedString m_str;
		std::pair<std::string, std::string> m_strSplit{};
		formattedString m_str2;
		std::pair<std::string, std::string> m_str2Split{};
		ImU32 m_lineColor{ IM_COL32(255, 255, 255, 255) };
		ImVec2 m_lineSize{ 0.001f, m_size.y };
	};
}