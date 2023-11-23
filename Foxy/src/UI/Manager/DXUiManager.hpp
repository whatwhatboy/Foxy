#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Pch/Common.hpp"
#include "DirectX/Renderer/Renderer.hpp"
#include "R.A.G.Engine/Natives/Natives.hpp"
#include "UI/Timer/Timer.hpp"
#include "UI/SmoothScroll/SmoothScroll.hpp"
#include "UI/Subs/Sub.hpp"
#include "UI/Abstracts/AbstractSub.hpp"
#include "UI/Abstracts/AbstractOpt.hpp"
#include "UI/Notify/Notify.hpp"

inline std::vector<std::string> getWords(std::string str) {
	std::vector<std::string> words;
	std::string word = "";
	for (int i = 0; i < str.length(); i++) {
		if (str.at(i) == ' ') {
			words.push_back(word);
			word.erase();
		}
		else {
			word += str.at(i);
			if (i == str.length() - 1)
				words.push_back(word);
		}
	}
	return words;
}
inline std::vector<std::string> getWordsSimple(std::string str) {
	std::vector<std::string> words;
	std::string word = "";
	for (auto& s : str) {
		if (s != ' ')
			word += s;
		else {
			words.push_back(word);
			word.clear();
		}
	}
	return words;
}
namespace base::ui {
	using namespace uiTypes;
	inline int rotationStartIdx{};
	inline int rotationStartIdx2{};
	inline ImVec2 ImRotationCenter(ImDrawList* drawList = ImGui::GetBackgroundDrawList()) {
		ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); //Bounds
		const auto& buf = drawList->VtxBuffer;
		for (int i{ rotationStartIdx }; i != buf.Size; ++i)
			l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);
		return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2);
	}
	inline void ImRotateStart() { rotationStartIdx = ImGui::GetBackgroundDrawList()->VtxBuffer.Size, rotationStartIdx2 = ImGui::GetForegroundDrawList()->VtxBuffer.Size; }
	inline void ImRotateEnd(float rad) {
		float s = sin(rad), c = cos(rad);
		ImVec2 center = ImRotationCenter();
		ImVec2 center2 = ImRotationCenter(ImGui::GetForegroundDrawList());
		center = ImRotate(center, s, c) - center;
		center2 = ImRotate(center2, s, c) - center2;
		auto& buf = ImGui::GetBackgroundDrawList()->VtxBuffer;
		auto& buf2 = ImGui::GetForegroundDrawList()->VtxBuffer;
		for (int i{ rotationStartIdx }; i != buf.Size; ++i)
			buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
		for (int i{ rotationStartIdx2 }; i != buf2.Size; ++i)
			buf2[i].pos = ImRotate(buf2[i].pos, s, c) - center2;
	}
	inline char g_timeBuf[80];
	inline std::vector<std::string> g_tooltips{};
	inline void addTooltip(std::string str) {
		g_tooltips.push_back(str);
	}
	inline void addTooltipsFromFile(fs::path file, int32_t numberOfTooltipsToAdd = -1) {
		std::ifstream f(file);
		std::string l{};
		while (std::getline(f, l)) {
			g_tooltips.emplace_back(l);
		}
		f.close();
	}
	inline void removeTooltip(std::string key) {
		for (size_t i{ g_tooltips.size() }; i; --i) {
			if (!g_tooltips[i].compare(key.data())) {
				g_tooltips.erase(g_tooltips.begin() + i);
				break;
			}
		}
	}
	class dxUiManager {
	public:
		//Handle Drawing
		void handleDrawing();
	public:
		//Draw Handlers
		// Header Handler
		void headerHandler();
		// SmoothScroll Handler
		smoothScroll smoothScrollHandler(bool selected);
		// Option Handler (Left Text)
		void optionHandler_Left(abstractOpt* opt);
		// Option Handler (Right Text)
		void optionHandler_Right(abstractOpt* opt);
		// Option Handler
		void optionHandler(abstractOpt* opt, bool selected);
		// Footer Handler
		void footerHandler(abstractSub* sub);
		// Description Handler
		void descriptionHandler();
		// Overlay Handler
		void overlayHandler();
		// Notify Handler
		void notifyHandler();
		// Open tooltip handler
		void openTooltipHandler();
	public:
		//Draw Helpers
		// Rect Helper
		void rect(ImVec2 pos, ImVec2 size, ImU32 color, float rounding = 0.f, ImDrawFlags flags = ImDrawFlags_None, ImDrawList* drawList = nullptr);
		// Rect Outline Helper
		void rectOutline(ImVec2 pos, ImVec2 size, ImU32 color, float thickness = 1.25f, ImDrawList* drawList = nullptr);
		// Line Helper
		void line(ImVec2 pos, ImVec2 size, ImU32 color, float thickness = 1.f, ImDrawList* drawList = nullptr);
		// Image Helper
		void image(ImTextureID image, ImVec2 pos, ImVec2 size, ImU32 color = IM_COL32(255, 255, 255, 255), ImDrawList* drawList = nullptr);
		// Text Helpers
		//  Default Text
		void text(std::string str, ImVec2 pos, ImU32 color, float wrap = -1.f, ImFont* font = g_renderer->m_optionFont, ImDrawList* drawList = nullptr);
		//  Left
		void leftText(std::string str, ImVec2 pos, ImU32 color, ImFont* font = g_renderer->m_optionFont, float wrap = -1.f, ImDrawList* drawList = nullptr);
		//  Center
		void centerText(std::string str, ImVec2 pos, ImU32 color, ImFont* font = g_renderer->m_optionFont, float wrap = -1.f, ImDrawList* drawList = nullptr);
		//  Right
		void rightText(std::string str, ImVec2 pos, ImU32 color, ImFont* font = g_renderer->m_optionFont, float wrap = -1.f, ImDrawList* drawList = nullptr);
		// Width Helper
		void width();
	public:
		//Input Handlers
		// Reset Input Handler
		void resetInputHandler();
		// Input Check Handler
		void inputCheckHandler();
		// Keypress Handler
		void keypressHandler();
	public:
		//Submenu Handlers
		// Sub Adding
		void addSub(sub&& submenu) {
			auto newSub = new sub(std::move(submenu));
			m_allSubmenus.emplace_back(newSub);
			if (m_submenuStack.empty())
				m_submenuStack.push(newSub);
		}
		// Sub Switching
		void switchToSub(subId subid) {
			for (auto&& sub : m_allSubmenus) {
				if (sub->getId().cmp(subid)) {
					m_submenuStack.push(sub.get());
					m_currentOption = 0;
					return;
				}
			}
		}
		// Get Sub Data
		sub* getSubData(subId subid) {
			for (auto&& sub : m_allSubmenus) {
				if (sub->getId().cmp(subid))
					return sub.get();
			}
			return nullptr;
		}
	public:
		//Notification handler
		// Does Exist Check
		bool doesNotifyExist(notifyTemplate ctx) {
			for (auto& n : m_notifications) {
				if (ctx.m_title == n.m_title && ctx.m_content == n.m_content) {
					n.m_creationTime = GetTickCount64();
					n.m_isRepeat = true;
					n.m_repeatCount++;
					return true;
				}
			}
			return false;
		}
		// Notification sender
		template <typename ...arguments>
		void sendNotify(std::string title, std::string format, arguments... args) {
			auto msg = std::vformat(format, std::make_format_args(args...));
			notifyTemplate ctx{ title, 3500, false, 0, msg };
			if (!doesNotifyExist(ctx))
				m_notifications.push_back(ctx);
		}
		// Draw hanlder
		void notify(notifyTemplate data);
		// Notification data array
		std::vector<notifyTemplate> m_notifications{};
		// Notification draw limit
		int m_notificationDrawLimit{ 10 };
		// Draw Base
		float m_notifyDrawBase{};
	public:
		//General Handlers (Called all across the ui).
		// Pos X + Y
		ImVec2 m_pos{ 0.13f, 0.04f };
		// Width
		float m_width{ 0.22f };
		// Draw base
		float m_drawBase{ 0.f };
		// Menu Scale
		float m_scale{ 1.f };
		// Padding
		float m_leftPadding{ 2.1f };
		float m_rightPadding{ 2.1f };
		// Menu Open
		bool m_open{ true };
		// Do A Barrel Roll
		bool m_doABarrelRoll{};
		// Open Tooltip
		bool m_drawOpenTooltip{ true };
		// Open Tooltip String
		std::string m_tooltip{};
		//Key Handlers
		// Keypress Handlers
		bool m_openKeyPressed{};
		bool m_backKeyPressed{};
		bool m_enterKeyPressed{};
		bool m_upKeyPressed{};
		bool m_downKeyPressed{};
		bool m_leftKeyPressed{};
		bool m_rightKeyPressed{};
		// Keypress Delay Handlers
		size_t m_openDelay{ 20 }; //Open/Close Delay
		size_t m_backDelay{ 20 }; //Back Delay
		size_t m_enterDelay{ 20 }; //Enter Delay
		size_t m_verticalDelay{ 10 }; //Left/Right Delay
		size_t m_horizontalDelay{ 10 }; //Up/Down Delay
		// Key handler
		void checkIfPressed(bool& value, int padIdx, int key, int keyboardKey, size_t delay);
		void checkIfPressed(bool& value, int padIdx, int key, int key2, int keyboardKey, size_t delay);
		// Basic key handler
		auto checkIfPressed(int key) { return (GetForegroundWindow() == FindWindowA("grcWindow", NULL)) && GetAsyncKeyState(key); }
		// Force lock input
		bool m_forceLockInput{};
	public:
		struct keyboardState {
			uint32_t m_time;
			bool m_isWithAlt{}, m_wasDownBefore{}, m_isUpNow{}, m_justUp{}, m_justDown{};
		};
		keyboardState m_keyStates[255]{};
		int m_lastKey{};
	public:
		//Sound Handlers
		// Queue Sound
		struct soundData {
			bool enabled{ true };
			std::string_view id{};
			std::string_view set{ "HUD_FRONTEND_DEFAULT_SOUNDSET" };
		};
		void soundQueue(soundData sound);
		// Sound disable
		struct sounds {
			sounds() : open({ true, "SELECT" }), close({ true, "BACK" }), enter(open), back(close), left({ true, "NAV_LEFT_RIGHT" }), right(left), up({ true, "NAV_UP_DOWN" }), down(up) {}
			soundData open{}, close{}, enter{}, back{}, left{}, right{}, up{}, down{};
		};
		sounds m_sounds{};
	public:
		//Header Handlers
		// Draw header text
		bool m_drawHeaderText{ true };
		// Draw header rect
		bool m_drawHeaderRect{ true };
		// Header loaded check
		bool m_hasHeaderLoaded{};
	public:
		//Option Handlers
		// Current option in op ount
		size_t m_currentOption{};
		// Full option count
		size_t m_optionCount{};
		// Max visble options
		int m_maxVisOptions{ 13 };
		// Smooth scroll
		smoothScroll m_scroll{};
		// Smooth scroll speed
		float m_scrollSpeed{ 0.5f };
	public:
		//Infobox Handlers
		// Infobox text
		std::string m_infoText{};
	public:
		//Submenu Handlers
		// Submenu List
		std::vector<std::unique_ptr<sub>> m_allSubmenus{};
		// Submenu Stack
		std::stack<sub*> m_submenuStack{};
	public:
		//Scaling Handlers
		// Font scale handler
		void scale();
		// Screen Resolution
		ImVec2 getRes() {
			return { 1920.f, 1080.f };
		}
		// Get pixel math (floating pont -> pixels)
		ImVec2 getPixelMathType(ImVec2 value) {
			return value * m_displaySize * m_scale;
		}
		// Get pixel math (floating pont -> pixels)
		ImVec2 getFloatingPointMathType(ImVec2 value) {
			return value / m_displaySize;
		}
		// Scale pos and size for rects (images, lines, etc)
		std::pair<ImVec2, ImVec2> scaleRectangle(ImVec2 pos, ImVec2 size) {
			return { getPixelMathType(pos - (size / 2.f)), getPixelMathType(size) };
		}
		// Scale text
		ImVec2 scaleText(ImFont* font, std::string text, float wrap = 0.f) {
			auto textSize = font->CalcTextSizeA(font->FontSize * font->Scale, FLT_MAX, wrap ? wrap * m_displaySize.x : -1.f, text.c_str(), NULL, NULL);
			textSize.x = IM_FLOOR(textSize.x + 0.99999f); //Round
			return { textSize.x / m_displaySize.x, textSize.y / m_displaySize.y };
		}
		// Scale the image according to 1920x1080
		ImVec2 scaleImage(float size) {
			//1080 / 1920
			return { 0.5625f * size, size };
		}
	public:
		//Stored ImGui Handles
		// IO
		ImGuiIO* m_imguiIO{};
		// Drawlists
		//  Background
		ImDrawList* m_backgroundDrawList{};
		//  Foreground
		ImDrawList* m_foregroundDrawList{};
		// Display Size
		ImVec2 m_displaySize{ getRes() };
		// Window Flags
		ImGuiWindowFlags m_windowFlags{
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground |
			ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs
		};
		// Cache function
		void cacheHandles();
	public:
		//Sizes 
		ImVec2 m_headerRectSize{ m_width, 0.093f }; //Header Size
		ImVec2 m_optionRectSize{ m_width, 0.032f }; //Option Size
		ImVec2 m_footerRectSize{ m_width, 0.034f }; //Footer Size
		ImVec2 m_infoboxRectSize{ m_width, 0.030f }; //Infobox Size
		float m_infoboxRectPadding{ 0.005f };
		ImVec2 m_infoboxSliverRectSize{ m_width, 0.002f }; //Infobox Sliver Size
		ImVec2 m_notifyRectSize{ 0.19f, 0.035f }; //Notify Size
		ImVec2 m_notifyLineSize{ 0.002f, 0.035f }; //Notify line Size
		ImVec2 m_overlayRectSize{ 0.205f, 0.03f }; //Overlay Size
		ImVec2 m_overlaySliverRectSize{ 0.205f, 0.002f }; //Overlay Sliver Size
		ImVec2 m_playerinfoRectSize{ m_width, 0.034f }; //Playerinfo Size
	public:
		//Colors
		ImU32 m_openTooltipTextColor = IM_COL32(160, 110, 220, 255); //Open tooltip text color
		ImU32 m_openTooltipKeyTextColor = IM_COL32(255, 255, 255, 255); //Open tooltip key text color
		ImU32 m_notifyRectColor = IM_COL32(24, 24, 24, 255); //Notify bg color
		ImU32 m_headerRectColor = IM_COL32(160, 110, 220, 255); //Header bg color
		ImU32 m_playerinfoRectColor = IM_COL32(10, 10, 10, 205); //Playerinfo bg color
		ImU32 m_playerinfoSliverRectColor = IM_COL32(145, 115, 205, 205); //Playerinfo sliver color
		ImU32 m_optionRectColor = IM_COL32(25, 25, 25, 170); //Option bg color
		ImU32 m_footerRectColor = IM_COL32(160, 110, 220, 255); //Footer bg color
		ImU32 m_infoboxRectColor = IM_COL32(10, 10, 10, 205); //Infobox bg color
		ImU32 m_infoboxSliverRectColor = IM_COL32(160, 110, 220, 255); //Infobox sliver color
		ImU32 m_overlayRectColor = IM_COL32(4, 4, 4, 210); //Overlay bg color
		ImU32 m_overlaySliverRectColor = IM_COL32(160, 130, 220, 255); //Overlay silver color
		ImU32 m_notifyLineColor = IM_COL32(160, 130, 220, 255); //Notift silver color
		ImU32 m_notifyLineFlashColor = IM_COL32(255, 255, 255, 255); //Notify flash silver color
		ImU32 m_notifyTextColor = IM_COL32(255, 255, 255, 255); //Notify txt color
		ImU32 m_headerTextColor = IM_COL32(255, 255, 255, 255); //Header txt color
		ImU32 m_playerinfoTextColor = IM_COL32(255, 255, 255, 255); //Playerinfo txt color
		ImU32 m_optionTextColor = IM_COL32(255, 255, 255, 255); //Option txt color
		ImU32 m_vectorOptionTextColor = IM_COL32(255, 255, 255, 170); //Vector option count txt color
		ImU32 m_footerTextColor = IM_COL32(255, 255, 255, 255); //Footer txt color (Text)
		ImU32 m_footerArrowsColor = IM_COL32(255, 255, 255, 255); //Footer txt color (arrows)
		ImU32 m_infoboxTextColor = IM_COL32(255, 255, 255, 255); //Infobox txt color
		ImU32 m_grayedTextColor = IM_COL32(80, 80, 80, 255); //Grayed txt color
		ImU32 m_optionSelectedRectColor = IM_COL32(145, 115, 205, 255); //Option selected bg color
	};
}
inline base::ui::dxUiManager g_dxUiManager;