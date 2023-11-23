#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include <functional>
#include "Opt.hpp"
#include "R.A.G.Engine/Colors.hpp"
#include "DirectX/Renderer/Renderer.hpp"

namespace base::uiTypes {
	inline void inputColor(std::string name, ImU32* color, std::function<void()> onChange = nullptr) {
		static auto txt = "##" + name + "_colInput";
		g_rendererQueue.add(txt, [name, color, onChange] {
			if (ImGui::Begin(txt.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground)) {
				ImGui::SetWindowSize(g_dxUiManager.getPixelMathType({ g_dxUiManager.m_width + 0.01f, 0.5f }));
				ImGui::SetWindowPos(g_dxUiManager.getPixelMathType({ g_dxUiManager.m_pos.x + g_dxUiManager.m_width + 0.005f, g_dxUiManager.m_pos.y - 0.005f }));
				ImGui::SetWindowFontScale(g_dxUiManager.m_scale);
				g_dxUiManager.m_forceLockInput = true;
				ImGui::GetIO().MouseDrawCursor = true;
				static float col[4] = {
					((*color >> IM_COL32_R_SHIFT) & 0xFF) * (1.0f / 255.0f),
					((*color >> IM_COL32_G_SHIFT) & 0xFF) * (1.0f / 255.0f),
					((*color >> IM_COL32_B_SHIFT) & 0xFF) * (1.0f / 255.0f),
					((*color >> IM_COL32_A_SHIFT) & 0xFF) * (1.0f / 255.0f) };
				ImGui::ColorPicker4(txt.c_str(), col, ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_DisplayRGB);
				*color = IM_COL32(int(col[0] * 255.f), int(col[1] * 255.f), int(col[2] * 255.f), int(col[3] * 255.f));
				if (g_dxUiManager.checkIfPressed(VK_SHIFT) && g_dxUiManager.checkIfPressed(VK_RETURN)) {
					if (onChange)
						onChange();
					ImGui::GetIO().MouseDrawCursor = false;
					g_dxUiManager.m_forceLockInput = false;
					g_rendererQueue.remove(txt);
				}
				else if (g_dxUiManager.checkIfPressed(VK_ESCAPE)) {
					ImGui::GetIO().MouseDrawCursor = false;
					g_dxUiManager.m_forceLockInput = false;
					g_rendererQueue.remove(txt);
				}
				ImGui::End();
			}
		});
	}
	class colorOpt : public opt<colorOpt> {
	public:
		colorOpt& setColor(ImU32* color, std::function<void()> customAction = nullptr) {
			m_customAction = customAction;
			m_callActionOnChange = m_customAction ? true : false;
			m_color = color;
			if (!getColorPtr())
				*m_color = IM_COL32(255, 255, 255, 255);
			if (auto col = ImGui::ColorConvertU32ToFloat4(getColor()); col.w == 0.f)
				*m_color = IM_COL32(int(col.x * 255.f), int(col.y * 255.f), int(col.z * 255.f), 255);
			return *this;
		}
		ImU32* getColorPtr() {
			return m_color;
		}
		ImU32 getColor() {
			return *getColorPtr();
		}
		RGBA getRGBAColor() {
			auto col = ImGui::ColorConvertU32ToFloat4(getColor());
			return RGBA(int(col.x * 255.f), int(col.y * 255.f), int(col.z * 255.f), int(col.w * 255.f));
		}
		void handleAction(eOptAction action) {
			switch (action) {
			case eOptAction::EnterPress: {
				if (m_callActionOnChange) {
					inputColor(getLeft(), m_color, m_customAction);
				}
				else {
					inputColor(getLeft(), m_color);
				}
				opt::handleAction(action);
			} break;
			}
		}
		eOptType getOptType() {
			return eOptType::ColorOpt;
		}
	private:
		bool m_callActionOnChange{};
		std::function<void()> m_customAction{};
		ImU32* m_color{};
	};
}