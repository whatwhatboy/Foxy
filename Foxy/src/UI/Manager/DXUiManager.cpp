#include "DXUiManager.hpp"
#include "DirectX/Renderer/Renderer.hpp"
#include "Script/Main/MainScript.hpp"
#include "Features/Features.hpp"
#include "UI/Options/ColorOpt.hpp"

namespace base::ui {
	void gotoParent() {
		if (g_dxUiManager.m_submenuStack.size() <= 1)
			g_dxUiManager.m_open = false;
		else
			g_dxUiManager.m_submenuStack.pop();
	}
}
namespace base::ui {
	void dxUiManager::handleDrawing() {
		//Needs to be done here or else it gives a flicker effect when alt tabbed/moving mouse cursor
		resetInputHandler();
		//Handle the scale
		scale();
		//Handle the width
		width();
		//Check if the menu is opened
		if (m_open) {
			if (!m_submenuStack.empty()) {
				auto sub = m_submenuStack.top(); //Grab the first sub off the submenu stack
				sub->resetOptCount(); //Clear the options
				//Queue the handled press in a different thread
				sub->executeOptAdding(); //Handle the actual execution of the press (Addon to the clear options)
				m_drawBase = m_pos.y;
				m_currentOption = sub->getSelectedOpt() + 1; //Set the current option
				headerHandler(); //Draw the header
				if (sub->getNumberOfOptions() != 0) { //Check if the number of options isn't 0
					size_t startPoint{}, endPoint{ sub->getNumberOfOptions() > m_maxVisOptions ? m_maxVisOptions : sub->getNumberOfOptions() };
					if (sub->getNumberOfOptions() > m_maxVisOptions && sub->getSelectedOpt() >= m_maxVisOptions - 1)
						startPoint = sub->getSelectedOpt() - m_maxVisOptions + 1, endPoint = sub->getSelectedOpt() + 1;
					for (size_t i{ startPoint }; i != endPoint; ++i)
						m_optionCount++, optionHandler(sub->getOpt(i), i == sub->getSelectedOpt());
				}
				footerHandler(sub); //Draw the footer
				descriptionHandler(); //Draw the description
			}
		}
		else {
			openTooltipHandler();
		}
		//Draw the overlay
		overlayHandler();
		//Draw the notifications
		notifyHandler();
	}
	int g_lastFrameCount{};
	void flipThroughFrames(int delay) {
		static timer tick(0ms);
		tick.setDelay(delay);
		if (tick.update() && g_lastFrameCount != ImGui::GetFrameCount()) {
			g_lastFrameCount = ImGui::GetFrameCount();
			if (g_renderer->m_header.size() != 1)
				if (g_renderer->m_headerFrame < g_renderer->m_header.size() - 1)
					g_renderer->m_headerFrame++;
				else if (g_renderer->m_headerFrame == g_renderer->m_header.size() - 1)
					g_renderer->m_headerFrame = 0;
				else
					g_renderer->m_headerFrame = 0;
		}
	}
	void dxUiManager::headerHandler() {
		if (m_hasHeaderLoaded) {
			flipThroughFrames(g_renderer->m_header[g_renderer->m_headerFrame].delay);
			//image(g_renderer->m_header[g_renderer->m_headerFrame].shader.resourceView, ImVec2(m_pos.x, m_drawBase + (m_headerRectSize.y / 2.f)), m_headerRectSize, m_headerRectColor);
		}
		else {
			if (m_drawHeaderRect) {
				rect(ImVec2(m_pos.x, m_drawBase + m_headerRectSize.y / 2.f), m_headerRectSize, m_headerRectColor);
			}
			if (m_drawHeaderText) {
				leftText(xorstr_(BRANDING_NAME), ImVec2(m_pos.x - (m_width / m_leftPadding), m_pos.y + (m_headerRectSize.y / 2.f) - (scaleText(g_renderer->m_headerFont, xorstr_(BRANDING_NAME)).y / 2.f)), m_headerTextColor, g_renderer->m_headerFont);
			}
		}
		m_drawBase += m_headerRectSize.y;
	}
	smoothScroll dxUiManager::smoothScrollHandler(bool isOpSelected) {
		const auto scrollPosition{ m_drawBase + (m_optionRectSize.y / 2.f) };
		static smoothScroll scroll(scrollPosition, float(m_scrollSpeed / 10.f));
		if (isOpSelected)
			scroll.setTarget(scrollPosition).update(float(m_scrollSpeed / 10.f));
		return scroll;
	}
	void dxUiManager::optionHandler_Left(abstractOpt* opt) {
		const auto text = opt->getLeft();
		switch (opt->getOptType()) {
		case eOptType::BreakOpt: {
			centerText(text, ImVec2(m_pos.x, m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_breakFont);
		} break;
		default: {
			leftText(text, ImVec2(m_pos.x - (m_width / m_leftPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
		} break;
		}
	}
	void dxUiManager::optionHandler_Right(abstractOpt* opt) {
		const auto text = opt->getRight();
		switch (opt->getOptType()) {
		case eOptType::IconOpt: {
			rightText(text, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_iconFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_iconFont);
		} break;
		case eOptType::SubOpt: {
			rightText(xorstr_("r"), ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_iconFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_iconFont);
		} break;
		case eOptType::BoolOpt: {
			rightText(text, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_iconFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_iconFont);
		} break;
		case eOptType::NumOpt: {
			rightText(text, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
		} break;
		case eOptType::NumBoolOpt: {
			//Get the text before the icon
			rightText(text.substr(0, text.find(" ")), ImVec2(m_pos.x + (m_width / m_rightPadding) - (0.005f), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
			//Actual icon
			rightText(text.substr(text.find(" ") + 1), ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_iconFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_iconFont);
		} break;
		case eOptType::VecOpt: {
			rightText(text, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
		} break;
		case eOptType::VecBoolOpt: {
			std::string icon{ text.substr(text.find(" ")) };
			rightText(text, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
			//rightText(text.substr(0, text.find(" ")), ImVec2(m_pos.x + (m_width / m_rightPadding) - (0.005f), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
			//rightText(text.substr(text.find(" ") + 1), ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_iconFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_iconFont);
		} break;
		//case eOptType::ColorOpt: {
		//	colorOpt* option = dynamic_cast<colorOpt*>(opt);
		//	if (option && option->getColorPtr())
		//		rect(ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f)), scaleImage(0.04f), option->getColor());
		//} break;
		default: {
			rightText(text, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_optionRectSize.y / 2.f) - (scaleText(g_renderer->m_optionFont, text).y / 2.f)), m_optionTextColor, g_renderer->m_optionFont);
		} break;
		}
	}
	void dxUiManager::optionHandler(abstractOpt* opt, bool selected) {
		rect(ImVec2(m_pos.x, m_drawBase + (m_optionRectSize.y / 2.f)), ImVec2(m_optionRectSize.x, m_optionRectSize.y), m_optionRectColor); //Option Background
		rect(ImVec2(m_pos.x, m_scroll.getCurrent()), m_optionRectSize, m_optionSelectedRectColor); //Scroller
		//Handle scrolling
		m_scroll = smoothScrollHandler(selected);
		//Left Text
		optionHandler_Left(opt);
		//Right Text
		optionHandler_Right(opt);
		m_drawBase += m_optionRectSize.y;
	}
	void dxUiManager::footerHandler(abstractSub* sub) {
		static const std::string& positionText{ std::format("{}/{}", m_currentOption, sub->getNumberOfOptions()) };
		rect(ImVec2(m_pos.x, m_drawBase + m_footerRectSize.y / 2.f), m_footerRectSize, m_footerRectColor);
		leftText(sub->getName(), ImVec2(m_pos.x - (m_width / m_leftPadding), m_drawBase + (m_footerRectSize.y / 2.f) - (scaleText(g_renderer->m_subtitleFont, positionText).y / 2.f)), m_optionTextColor, g_renderer->m_subtitleFont);
		if (sub->getSelectedOpt() == 0) {
			centerText(xorstr_("u"), ImVec2(m_pos.x, m_drawBase + (m_footerRectSize.y / 2.f) - (scaleText(g_renderer->m_bigIconFont, xorstr_("u")).y / 2.f)), m_optionTextColor, g_renderer->m_bigIconFont);
		}
		else if (sub->getSelectedOpt() == sub->getNumberOfOptions()) {
			centerText("u\nd", ImVec2(m_pos.x, m_drawBase + (m_footerRectSize.y / 2.f) - (scaleText(g_renderer->m_bigIconFont, xorstr_("u\nd")).y / 2.f)), m_optionTextColor, g_renderer->m_bigIconFont);
		}
		else {
			centerText(xorstr_("d"), ImVec2(m_pos.x, m_drawBase + (m_footerRectSize.y / 2.f) - (scaleText(g_renderer->m_bigIconFont, xorstr_("d")).y / 2.f)), m_optionTextColor, g_renderer->m_bigIconFont);
		}
		rightText(positionText, ImVec2(m_pos.x + (m_width / m_rightPadding), m_drawBase + (m_footerRectSize.y / 2.f) - (scaleText(g_renderer->m_subtitleFont, positionText).y / 2.f)), m_optionTextColor, g_renderer->m_subtitleFont);
		m_drawBase += m_footerRectSize.y;
	}
	void dxUiManager::descriptionHandler() {
		if (!m_submenuStack.empty()) {
			if (const auto& sub = m_submenuStack.top(); sub->getNumberOfOptions() != 0) {
				if (auto opt = sub->getOpt(sub->getSelectedOpt())) {
					if (auto descText = opt->getDescription(); !descText.empty()) {
						const auto wrap{ m_infoboxRectSize.x };
						const auto textSize = scaleText(g_renderer->m_infoboxFont, descText);
						m_drawBase += m_infoboxRectPadding;
						rect(ImVec2(m_pos.x, m_drawBase + m_infoboxRectSize.y / 2.f), { m_infoboxRectSize.x, m_infoboxRectSize.y }, m_infoboxRectColor);
						rect(ImVec2(m_pos.x, m_drawBase + m_infoboxSliverRectSize.y / 2.f), m_infoboxSliverRectSize, m_infoboxSliverRectColor);
						leftText(descText, ImVec2(m_pos.x - (m_width / m_leftPadding), m_drawBase + (m_footerRectSize.y / 2.f) - (textSize.y / 2.f)), m_infoboxTextColor, g_renderer->m_infoboxFont);
						m_drawBase += m_infoboxRectSize.y;
					}
				}
			}
		}
	}
	void dxUiManager::overlayHandler() {
		time_t epochTime{ time(nullptr) };
		auto localTime{ std::localtime(&epochTime) };
		#ifdef DEBUG_MODE
		strftime(g_timeBuf, sizeof(g_timeBuf), "%c", localTime);
		#else
		strftime(g_timeBuf, sizeof(g_timeBuf), "%T", localTime);
		#endif
		rect(ImVec2(0.897f, 0.02f), m_overlayRectSize, m_overlayRectColor);
		rect(ImVec2(0.897f, 0.005f), m_overlaySliverRectSize, m_overlaySliverRectColor);
		leftText(std::format(LOWER_BRANDING_NAME"cheats.lol | FPS {} | {}", int(ImGui::GetIO().Framerate / 1.5f), g_timeBuf), ImVec2(0.799f, (m_overlayRectSize.y / 2.f) - 0.005f), IM_COL32(255, 255, 255, 255), g_renderer->m_font);
	}
	void dxUiManager::openTooltipHandler() {
		if (m_tooltip.empty()) {
			m_tooltip = g_tooltips.empty() ? xorstr_("foxycheats.one [No Tooltips!]") : g_tooltips[rng(NULL, g_tooltips.size() - 1).get<size_t>()];
		}
		if (m_drawOpenTooltip) {
			centerText(m_tooltip, { 0.5f - scaleText(g_renderer->m_openTooltipFont, m_tooltip).x, 0.06f }, m_openTooltipTextColor, g_renderer->m_openTooltipFont);
			std::string keyText{ g_isUsingKeyboard ? xorstr_("Insert") : xorstr_("RB + Right") };
			centerText(keyText, ImVec2(0.5f, 0.09f), m_openTooltipKeyTextColor, g_renderer->m_openTooltipFont);
		}
	}
	void dxUiManager::notifyHandler() {
		m_notifyDrawBase = m_overlayRectSize.y + 0.01f;
		for (int16_t i{}; i < m_notifications.size() && i <= m_notificationDrawLimit; ++i) {
			notify(m_notifications[i]);
			if (m_notifications[i].phase() == notifyPhase::expired) {
				m_notifications.erase(m_notifications.begin() + i);
			}
		}
	}
	void dxUiManager::notify(notifyTemplate data) {
		auto lineColor = m_notifyLineColor;
		if (data.m_isRepeat)
			data.setFlash(lineColor, m_notifyLineFlashColor);
		const float x = 0.9042f;
		const float wrap = m_notifyRectSize.x - 0.005f;
		const float textHeight = scaleText(g_renderer->m_font, data.m_content, wrap).y - (m_notifyRectSize.y / 2.f);
		m_notifyDrawBase += 0.004f;
		rect(ImVec2(x, m_notifyDrawBase + m_notifyRectSize.y / 2.f), ImVec2(m_notifyRectSize.x, m_notifyRectSize.y), m_notifyRectColor);
		rect(ImVec2(x - (m_notifyRectSize.x / 2.f), m_notifyDrawBase + m_notifyLineSize.y / 2.f), ImVec2(m_notifyLineSize.x, m_notifyLineSize.y), lineColor);
		leftText(data.m_title, ImVec2(x - (m_notifyRectSize.x / 2.f) + 0.004f, m_notifyDrawBase + ((m_notifyRectSize.y) / 2.f) - (scaleText(g_renderer->m_notifyFont, data.m_title).y / 2.f) - 0.007f), m_notifyTextColor, g_renderer->m_notifyFont, wrap);
		leftText(data.m_content, ImVec2(x - (m_notifyRectSize.x / 2.f) + 0.004f, m_notifyDrawBase + ((m_notifyRectSize.y) / 2.f) - (scaleText(g_renderer->m_font, data.m_title).y / 2.f) + 0.007f), m_notifyTextColor, g_renderer->m_font, wrap);
		m_notifyDrawBase += m_notifyRectSize.y;
	}
	void dxUiManager::rect(ImVec2 pos, ImVec2 size, ImU32 color, float rounding, ImDrawFlags flags, ImDrawList* drawList) {
		if (!drawList)
			drawList = m_backgroundDrawList;
		const auto values = scaleRectangle(pos, size);
		drawList->AddRectFilled(values.first, values.first + values.second, color, rounding, flags);
	}
	void dxUiManager::rectOutline(ImVec2 pos, ImVec2 size, ImU32 color, float thickness, ImDrawList* drawList) {
		if (!drawList)
			drawList = m_backgroundDrawList;
		const auto values = scaleRectangle(pos, size);
		drawList->AddRect(values.first, values.first + values.second, color, 0.f, 15, thickness);
	}
	void dxUiManager::line(ImVec2 pos, ImVec2 size, ImU32 color, float thickness, ImDrawList* drawList) {
		if (!drawList)
			drawList = m_backgroundDrawList;
		const auto values = scaleRectangle(pos, size);
		drawList->AddLine(values.first, values.first + values.second, color, thickness);
	}
	void dxUiManager::image(ImTextureID image, ImVec2 pos, ImVec2 size, ImU32 color, ImDrawList* drawList) {
		if (!drawList)
			drawList = m_foregroundDrawList;
		const auto values = scaleRectangle(pos, size);
		drawList->AddImage(image, values.first, values.first + values.second, ImVec2(0, 0), ImVec2(1, 1), color);
	}
	void dxUiManager::text(std::string str, ImVec2 pos, ImU32 color, float wrap, ImFont* font, ImDrawList* drawList) {
		if (!drawList)
			drawList = m_foregroundDrawList;
		drawList->AddText(font, font->FontSize * font->Scale, pos, color, str.c_str(), NULL, wrap);
	}
	void dxUiManager::leftText(std::string str, ImVec2 pos, ImU32 color, ImFont* font, float wrap, ImDrawList* drawList) {
		const ImVec2 scaledPos{ (pos * m_scale) * m_displaySize };
		text(str, scaledPos, color, wrap * m_displaySize.x, font, drawList);
	}
	void dxUiManager::centerText(std::string str, ImVec2 pos, ImU32 color, ImFont* font, float wrap, ImDrawList* drawList) {
		const ImVec2 scaledPos{ (pos * m_scale) * m_displaySize };
		text(str, { scaledPos.x - (scaleText(font, str, wrap).x / 2.f), scaledPos.y }, color, wrap * m_displaySize.x, font, drawList);
	}
	void dxUiManager::rightText(std::string str, ImVec2 pos, ImU32 color, ImFont* font, float wrap, ImDrawList* drawList) {
		const ImVec2 scaledPos{ (pos * m_scale) * m_displaySize };
		text(str, { scaledPos.x - (ImGui::CalcTextSize(str.c_str()).x), scaledPos.y }, color, wrap * m_displaySize.x, font, drawList);
	}
	void dxUiManager::resetInputHandler() {
		m_openKeyPressed = false;
		m_backKeyPressed = false;
		m_enterKeyPressed = false;
		m_upKeyPressed = false;
		m_downKeyPressed = false;
		m_leftKeyPressed = false;
		m_rightKeyPressed = false;
		m_optionCount = 0;
	}
	void dxUiManager::inputCheckHandler() {
		if (!m_forceLockInput && GetForegroundWindow() == pointers::g_hwnd) {
			checkIfPressed(m_openKeyPressed, 2, 227, 175, VK_INSERT, m_openDelay); //RB + R-Arrow
			checkIfPressed(m_backKeyPressed, 2, 194, VK_BACK, m_enterDelay); //A
			checkIfPressed(m_enterKeyPressed, 2, 191, VK_RETURN, m_backDelay); //B
			checkIfPressed(m_upKeyPressed, 2, 172, VK_UP, m_verticalDelay); //U-Arrow
			checkIfPressed(m_downKeyPressed, 2, 173, VK_DOWN, m_verticalDelay); //D-Arrow
			checkIfPressed(m_leftKeyPressed, 2, 174, VK_LEFT, m_horizontalDelay); //L-Arrow
			checkIfPressed(m_rightKeyPressed, 2, 175, VK_RIGHT, m_horizontalDelay); //R-Arrow
		}
	}
	void dxUiManager::checkIfPressed(bool& value, int padIdx, int key, int keyboardKey, size_t delay) {
		static simpleTimer t{};
		if (GetAsyncKeyState(keyboardKey) & 1
			&& PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx)
			|| PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key)
			&& !PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx))
		{
			value = true;
		}
		else if (GetAsyncKeyState(keyboardKey) & 0x8000
			&& PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx)
			|| PAD::IS_DISABLED_CONTROL_PRESSED(padIdx, key)
			&& !PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key)
			&& !PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx))
		{
			t.start(delay * 100);
			if (t.isReady())
				value = true;
		}
		else {
			t.reset();
		}
	}
	void dxUiManager::checkIfPressed(bool& value, int padIdx, int key, int key2, int keyboardKey, size_t delay) {
		static simpleTimer t{};
		if (GetAsyncKeyState(keyboardKey) & 1
			|| ((PAD::IS_DISABLED_CONTROL_PRESSED(padIdx, key)
				|| PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key))
				&& PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key2)
				&& !PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx)))
		{
			value = true;
		}
		else if (GetAsyncKeyState(keyboardKey) & 0x8000
			&& PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx)
			|| PAD::IS_DISABLED_CONTROL_PRESSED(padIdx, key)
			&& !PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key)
			&& PAD::IS_DISABLED_CONTROL_JUST_PRESSED(padIdx, key2)
			&& !PAD::IS_USING_KEYBOARD_AND_MOUSE(padIdx))
		{
			t.start(delay * 100);
			if (t.isReady())
				value = true;
		}
		else {
			t.reset();
		}
	}
	void dxUiManager::soundQueue(soundData sound) {
		g_fiberPool.queue([=] {
			if (sound.enabled)
				AUDIO::PLAY_SOUND_FRONTEND(-1, sound.id.data(), sound.set.data(), FALSE);
		});
	}
	void dxUiManager::keypressHandler() {
		if (m_openKeyPressed) {
			soundQueue(m_open ? m_sounds.open : m_sounds.close);
			m_open ^= true;
		}
		if (m_open && !m_submenuStack.empty()) {
			auto sub = m_submenuStack.top();
			if (sub->getNumberOfOptions()) {
				if (m_enterKeyPressed) {
					soundQueue(m_sounds.enter);
					if (auto opt = sub->getOpt(sub->getSelectedOpt()))
						opt->handleAction(eOptAction::EnterPress);
				}
				if (m_backKeyPressed) {
					soundQueue(m_sounds.back);
					gotoParent();
				}
				if (m_upKeyPressed) {
					soundQueue(m_sounds.up);
					sub->moveUp();
				}
				if (m_downKeyPressed) {
					soundQueue(m_sounds.down);
					sub->moveDown();
				}
				if (m_leftKeyPressed) {
					soundQueue(m_sounds.left);
					if (auto opt = sub->getOpt(sub->getSelectedOpt()))
						opt->handleAction(eOptAction::LeftPress);
				}
				if (m_rightKeyPressed) {
					soundQueue(m_sounds.right);
					if (auto opt = sub->getOpt(sub->getSelectedOpt()))
						opt->handleAction(eOptAction::RightPress);
				}
			}
			else {
				if (m_backKeyPressed) {
					soundQueue(m_sounds.back);
					gotoParent();
				}
			}
		}
	}
	void dxUiManager::scale() {
		m_imguiIO->FontAllowUserScaling = true;
		g_renderer->m_headerFont->Scale = m_scale + .5f;
		g_renderer->m_subtitleFont->Scale = m_scale;
		g_renderer->m_optionFont->Scale = m_scale;
		g_renderer->m_breakFont->Scale = m_scale;
		g_renderer->m_iconFont->Scale = m_scale;
		g_renderer->m_bigIconFont->Scale = m_scale;
		g_renderer->m_footerFont->Scale = m_scale;
		g_renderer->m_infoboxFont->Scale = m_scale;
	}
	void dxUiManager::cacheHandles() {
		if (m_backgroundDrawList != ImGui::GetBackgroundDrawList())
			m_backgroundDrawList = ImGui::GetBackgroundDrawList();
		if (m_foregroundDrawList != ImGui::GetForegroundDrawList())
			m_foregroundDrawList = ImGui::GetForegroundDrawList();
		if (!m_imguiIO)
			m_imguiIO = &ImGui::GetIO();
		m_displaySize = m_imguiIO->DisplaySize;
	}
	void dxUiManager::width() {
		m_headerRectSize.x = m_width;
		m_optionRectSize.x = m_width;
		m_footerRectSize.x = m_width;
		m_infoboxRectSize.x = m_width;
	}
}