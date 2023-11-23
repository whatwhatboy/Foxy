#pragma once
#include "UI/Information/SingleLine.hpp"
#include "UI/Information/DoubleLine.hpp"
#include "UI/Information/FormattedString.hpp"

namespace base::ui::informational {
	struct box {
		box(dxUiManager& mgr, formattedString title, ImVec2 pos, float offset, int align) : m_mgr(&mgr), m_pos(pos), m_offset(offset) {
			switch (align) {
			case 0: m_left = title.str(); break;
			case 1: m_center = title.str(); break;
			case 2: m_right = title.str(); break;
			}
		}
		box(dxUiManager& mgr, formattedString title, ImVec2 pos, float offset) : box(mgr, title, pos, offset, 0) {}
		box(dxUiManager& mgr, formattedString title, ImVec2 pos) : box(mgr, title, pos, 0.f) {}
		box(formattedString title, ImVec2 pos, float offset, int align) : box(g_dxUiManager, title, pos, offset, align) {}
		box(formattedString title, ImVec2 pos, float offset) : box(title, pos, offset, 0) {}
		box(formattedString title, ImVec2 pos) : box(title, pos, 0.f) {}

		void draw(dxUiManager* mgr, float offset, float& drawBase) {
			ImFont* font{ g_renderer->m_playerinfoFont };
			ImVec2 pos{ m_pos.x + offset, drawBase };
			mgr->rect(ImVec2(pos.x, pos.y + m_size.y / 2.f), m_size, m_bgColor);
			mgr->rect(ImVec2(pos.x, pos.y + m_sliverSize.y / 2.f), m_sliverSize, m_sliverColor);
			pos.y += m_sliverSize.y;
			if (!m_left.empty())
				mgr->leftText(m_left, ImVec2(pos.x - (m_size.x / 2.1f), pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_left).y / 2.f)), m_txtColor, font);
			if (!m_center.empty())
				mgr->centerText(m_center, ImVec2(pos.x, pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_center).y / 2.f)), m_txtColor, font);
			if (!m_right.empty())
				mgr->rightText(m_right, ImVec2(pos.x + (m_size.x / 2.1f), pos.y + (m_size.y / 2.f) - (mgr->scaleText(font, m_right).y / 2.f)), m_txtColor, font);
			drawBase += m_size.y;
		}
		void call() {
			if (m_lines.size()) {
				m_drawBase = m_mgr->m_pos.y;
				draw(m_mgr, m_offset, m_drawBase);
				for (auto& l : m_lines) {
					l->draw(m_mgr, m_pos, m_offset, m_drawBase);
				}
				m_lines.clear();
			}
		}
		template<typename t>
		void add(t l) {
			m_lines.emplace_back(new t(l));
		}

		std::string m_left{};
		std::string m_center{};
		std::string m_right{};
		dxUiManager* m_mgr{};
		std::vector<line*> m_lines{};
		ImVec2 m_pos{};
		float m_offset{};
		float m_drawBase{};
		ImU32 m_bgColor{ IM_COL32(10, 10, 10, 205) };
		ImU32 m_sliverColor{ IM_COL32(145, 115, 205, 255) };
		ImU32 m_txtColor{ IM_COL32(255, 255, 255, 255) };
		ImVec2 m_size{ 0.22f, 0.028f };
		ImVec2 m_sliverSize{ m_size.x, 0.005f };
	};
}