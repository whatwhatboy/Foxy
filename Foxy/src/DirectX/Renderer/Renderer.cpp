#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STB/STBImage.hpp"
#include "STB/STBImageWrite.hpp"
#include "Memory/Patterns/Pointers.hpp"
#include "Hooking/Hooking.hpp"
#include "UI/Manager/DXUiManager.hpp"
#include "Script/Main/MainScript.hpp"
#include <d3dcompiler.h>
#include "Fibers/Fiber.hpp"
#include "Fibers/FiberMgr.hpp"
#include "Fibers/DXFiberMgr.hpp"
#include "Fibers/FiberPool.hpp"
#include "R.A.G.Engine/SysMemAllocator.hpp"
#include "DirectX/Fonts/IconFontData.hpp"
#include "DirectX/Fonts/HeaderFontData.hpp"
#include "Renderer.hpp"
#include "Memory/Module.hpp"
#include "Memory/Util.hpp"
#include "ScriptHookV/ScriptHookV.hpp"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace base {
	namespace stb {
		namespace memory {
			imageData write(vec2& size, int comp, const void* data, int strideBytes, uint32_t delay) {
				int imageSize;
				stbi_uc* image{ stbi_write_png_to_mem((const unsigned char*)data, strideBytes, size.x.i32, size.y.i32, comp, &imageSize) };
				RETURN_IF_VALID(image, imageSize, delay)
			}
			image read(imageData data) {
				vec2 size{};
				stbi_uc* image{ stbi_load_from_memory(data.bytes, data.size, &size.x.i32, &size.y.i32, NULL, 0) };
				RETURN_IF_VALID(image, size)
			}
			gif readGif(imageData data) {
				vec2 size{};
				int* delays{};
				int frameCount{}, comp{};
				stbi_uc* image{ stbi_load_gif_from_memory(data.bytes, data.size, &delays, &size.x.i32, &size.y.i32, &frameCount, &comp, NULL) };
				RETURN_IF_VALID(image, data, delays, frameCount, size, comp)
			}
		}
		namespace file {
			image readImage(fs::path path) {
				vec2 size{};
				stbi_uc* image{ stbi_load(path.string().c_str(), &size.x.i32, &size.y.i32, NULL, 4) };
				RETURN_IF_VALID(image, size)
			}
			gif readGif(fs::path path) {
				FILE* f{ fopen(path.string().c_str(), xorstr_("rb")) };
				IS_VALID(f)
				imageData data{};
				fseek(f, 0L, SEEK_END);
				data.size = ftell(f);
				fseek(f, 0L, SEEK_SET);
				data.bytes = decltype(data.bytes)(malloc(data.size));
				fread(data.bytes, data.size, 1, f);
				fclose(f);
				gif gif{ memory::readGif(data) };
				free(data.bytes);
				return gif;
			}
		}
		std::vector<imageData> splitGif(fs::path path) {
			std::vector<imageData> frames{};
			gif gif{ file::readGif(path) };
			size_t bytes = gif.size.x.u64 * gif.comp;
			for (size_t i{}; i != gif.frameCount; ++i)
				frames.push_back(memory::write(gif.size, gif.comp, gif.bytes + bytes * gif.size.y.i32 * i, 0, gif.delays[i]));
			stbi_image_free(gif.bytes);
			return frames;
		}
	}
	namespace imgui {
		void setColors() {
			auto&& style = ImGui::GetStyle();
			auto& colors = style.Colors;
			style.Alpha = 1.f;
			style.WindowPadding = ImVec2(8, 8);
			style.PopupRounding = 0.f;
			style.FramePadding = { 8.f, 4.f };
			style.ItemSpacing = ImVec2(8, 4);
			style.ItemInnerSpacing = ImVec2(4, 4);
			style.TouchExtraPadding = { 0.f, 0.f };
			style.IndentSpacing = 21.f;
			style.ScrollbarSize = 5.f;
			style.ScrollbarRounding = 8.f;
			style.GrabMinSize = 8.f;
			style.GrabRounding = 4.25f;
			style.WindowBorderSize = 0.f;
			style.ChildBorderSize = 0.f;
			style.PopupBorderSize = 0.f;
			style.FrameBorderSize = 0.f;
			style.TabBorderSize = 0.f;
			style.WindowRounding = 0.f;
			style.ChildRounding = 3.f;
			style.FrameRounding = 4.25f;
			style.TabRounding = 2.f;
			style.MouseCursorScale = 0.7f;
			style.WindowTitleAlign = { 0.0f, 0.5f };
			style.ButtonTextAlign = { 0.5f, 0.5f };
			style.DisplaySafeAreaPadding = { 1.f, 1.f };
			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(1.00f, 0.90f, 0.19f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.50f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.50f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.21f, 0.21f, 0.21f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.78f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.28f, 0.27f, 0.27f, 0.54f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.85f, 0.70f, 0.85f, 0.90f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.70f, 0.85f, 0.90f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.95f, 0.80f, 0.95f, 0.51f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(0.39f, 0.38f, 0.38f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.41f, 0.41f, 0.41f, 0.74f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.49f, 0.49f, 0.78f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.55f, 0.87f);
			colors[ImGuiCol_Header] = ImVec4(0.37f, 0.37f, 0.37f, 0.31f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.38f, 0.38f, 0.38f, 0.37f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.37f, 0.37f, 0.37f, 0.51f);
			colors[ImGuiCol_Separator] = ImVec4(0.38f, 0.38f, 0.38f, 0.50f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.50f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.64f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
			colors[ImGuiCol_TabActive] = ImVec4(0.34f, 0.34f, 0.34f, 0.86f);
			colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.10f, 0.97f);
			colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
			colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		}
		void createHandles(comPtr<ID3D11Device> dev, comPtr<ID3D11DeviceContext> ctx) {
			ImGui::CreateContext();
			imgui::setColors();
			ImGui_ImplDX11_Init(dev.Get(), ctx.Get());
			ImGui_ImplWin32_Init(pointers::g_hwnd);
		}
		void createFont(ImFontAtlas* atlas, ImFont*& font, fs::path path, float size, ImFontConfig* cfg) {
			font = atlas->AddFontFromFileTTF(path.string().c_str(), size * g_dxUiManager.m_displaySize.x, cfg);
		}
		template <size_t dataSize>
		void createFont(ImFontAtlas* atlas, ImFont*& font, uint8_t(&data)[dataSize], float size, ImFontConfig* cfg) {
			font = atlas->AddFontFromMemoryTTF(data, dataSize, size * g_dxUiManager.m_displaySize.x, cfg);
		}
	}
	renderer::renderer() : m_swapchain(*pointers::g_swapchain) {
		m_wndProc = WNDPROC(SetWindowLongPtrW(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(&renderer::wndProc)));
		if (FAILED(m_swapchain->GetDevice(__uuidof(ID3D11Device), (void**)m_device.GetAddressOf())))
			throw std::runtime_error(xorstr_("Failed to create a ID3D11Device"));
		m_device->GetImmediateContext(m_context.GetAddressOf());
		m_stateSaver = std::make_unique<stateSaver>(m_context.Get());
		m_commonState = std::make_unique<DirectX::CommonStates>(m_device.Get());
		m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context.Get());
		imgui::createHandles(m_device, m_context);
		if (!g_dxUiManager.m_imguiIO)
			g_dxUiManager.m_imguiIO = &ImGui::GetIO();
		g_dxUiManager.cacheHandles();
		RECT handle{};
		if (GetWindowRect(FindWindowA("grcWindow", nullptr), &handle)) {
			m_window.get(handle);
		}
		g_dxUiManager.m_displaySize = m_window.size;
		m_fontCfg.FontDataOwnedByAtlas = false;
		ImFontAtlas* atlas{ ImGui::GetIO().Fonts };
		fs::path path{ fs::path(xorstr_("C:")).append(xorstr_("Windows")).append(xorstr_("Fonts")) };
		imgui::createFont(atlas, m_font, fs::path(path).append(xorstr_("Arial.ttf")), 0.009375f, &m_fontCfg);
		imgui::createFont(atlas, m_breakFont, fs::path(path).append(xorstr_("ArialI.ttf")), 0.009375f, &m_fontCfg);
		imgui::createFont(atlas, m_openTooltipFont, fs::path(path).append(xorstr_("Tahoma.ttf")), 0.021875f, &m_fontCfg);
		imgui::createFont(atlas, m_notifyFont, fs::path(path).append(xorstr_("ArialBD.ttf")), 0.0094f, &m_fontCfg);
		imgui::createFont(atlas, m_headerFont, g_headerFontData, 0.02605f, &m_fontCfg);
		imgui::createFont(atlas, m_subtitleFont, fs::path(path).append(xorstr_("ArialBD.ttf")), 0.0094f, &m_fontCfg);
		imgui::createFont(atlas, m_optionFont, fs::path(path).append(xorstr_("Arial.ttf")), 0.0094f, &m_fontCfg);
		imgui::createFont(atlas, m_playerinfoFont, fs::path(path).append(xorstr_("ArialBD.ttf")), 0.01779f, &m_fontCfg);
		imgui::createFont(atlas, m_footerFont, fs::path(path).append(xorstr_("ArialI.ttf")), 0.009375f, &m_fontCfg);
		imgui::createFont(atlas, m_infoboxFont, fs::path(path).append(xorstr_("Arial.ttf")), 0.0094f, &m_fontCfg);
		imgui::createFont(atlas, m_smallIconFont, g_iconFontData, 0.0052f, &m_fontCfg);
		imgui::createFont(atlas, m_iconFont, g_iconFontData, 0.007813f, &m_fontCfg);
		imgui::createFont(atlas, m_bigIconFont, g_iconFontData, 0.009475f, &m_fontCfg);
	}
	using namespace shv;
	renderer::~renderer() {
		resetTextureData();
		m_stateSaver.reset();
		m_commonState.reset();
		m_spriteBatch.reset();
		SetWindowLongPtrW(pointers::g_hwnd, GWLP_WNDPROC, LONG_PTR(m_wndProc));
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void renderer::present() {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		g_dxUiManager.cacheHandles();
		g_dxFiberMgr.tick();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		createTextures();
		if (!g_drawTextureArray.empty()) {
			m_spriteBatch->Begin(DirectX::SpriteSortMode::SpriteSortMode_BackToFront, m_commonState->NonPremultiplied());
			m_stateSaved = m_stateSaver->save(m_context.Get());
			for (auto& t : g_drawTextureArray) {
				if (t.tex.enabled)
					t.tex.draw(*m_spriteBatch);
			}
			if (!m_stateSaved)
				m_stateSaver->restore();
		}
		for (auto& cb : g_presentCallbacks) {
			m_stateSaved = m_stateSaver->save(m_context.Get());
			cb(m_swapchain.Get());
			if (!m_stateSaved)
				m_stateSaver->restore();
		}
	}
	LRESULT renderer::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (g_renderer.get()) {
			if (msg == WM_KEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP) {
				DWORD key{ DWORD(wParam) };
				WORD repeats{ WORD(lParam & 0xFFFF) };
				BYTE scanCode{ BYTE((lParam >> 16) & 0xFF) };
				BOOL isExt{ (lParam >> 24) & 1 }, isAlt{ msg == WM_SYSKEYDOWN || msg == WM_SYSKEYUP },
					wasDown{ (lParam >> 30) & 1 }, isUp{ msg == WM_SYSKEYUP || msg == WM_KEYUP };
				for (auto& fn : g_keyboardFunctions) {
					fn(key, repeats, scanCode, isExt, isAlt, wasDown, isUp);
				}
			}
			if (ImGui::GetCurrentContext())
				ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
			return CallWindowProcW(g_renderer->m_wndProc, hWnd, msg, wParam, lParam);
		}
		return (LRESULT)-1;
	}
	std::pair<shaderData, vec2> renderer::createTexture(fs::path path) {
		image image{ stb::file::readImage(path) };
		return std::make_pair(createShaderData(image), image.size);
	}
	std::vector<frameData> renderer::createGifTexture(fs::path path) {
		std::vector<frameData> frames{};
		std::vector<imageData> gif{ stb::splitGif(path) };
		for (auto& fr : gif) {
			image image{ stb::memory::read(fr) };
			frames.push_back({ fr.delay, createShaderData(image) });
		}
		return frames;
	}
	shaderData renderer::createShaderData(image image) {
		IS_VALID(image.data);
		ID3D11ShaderResourceView* resourceView{};
		ID3D11Texture2D* texture{};
		D3D11_TEXTURE2D_DESC desc{ image.size.x.u32, image.size.y.u32, 1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, { 1 }, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0 };
		D3D11_SUBRESOURCE_DATA subResource{ image.data, desc.Width * 4, 0 };
		m_device->CreateTexture2D(&desc, &subResource, &texture);
		IS_VALID(texture);
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{ DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_SRV_DIMENSION_TEXTURE2D, { 0, desc.MipLevels } };
		m_device->CreateShaderResourceView(texture, &srvDesc, std::add_pointer_t<decltype(resourceView)>(&resourceView));
		RETURN_IF_VALID(texture, resourceView);
	}
	//SHV Shit
	void renderer::createTextures() {
		for (auto& e : g_createTextureArray) {
			dxTexture2DWrapper t{};
			std::wstring name{ e.second.begin(), e.second.end() };
			comPtr<ID3D11ShaderResourceView>& view{ t.view };
			comPtr<ID3D11Resource>& resource{ t.resource };
			if (SUCCEEDED(CreateWICTextureFromFile(m_device.Get(), name.data(), resource.GetAddressOf(), view.GetAddressOf(), 0))) {
				t.resource.As(&t.texture);
				t.texture->GetDesc(&t.desc);
				scriptTex sTmp{};
				sTmp.id = e.first;
				sTmp.tex = t;
				g_drawTextureArray.push_back(sTmp);
				reloadTex rTmp{};
				rTmp.id = e.first;
				rTmp.name = strToWstr(e.second);
				g_reloadArray.push_back(rTmp);
			}
		}
		g_createTextureArray.clear();
	}
	void renderer::reloadTextures() {
		for (auto& e : g_reloadArray) {
			dxTexture2DWrapper t{};
			comPtr<ID3D11ShaderResourceView>& view{ t.view };
			comPtr<ID3D11Resource>& resource{ t.resource };
			if (SUCCEEDED(CreateWICTextureFromFile(m_device.Get(), e.name.data(), resource.GetAddressOf(), view.GetAddressOf(), 0))) {
				t.resource.As(&t.texture);
				t.texture->GetDesc(&t.desc);
				scriptTex sTemp{};
				sTemp.id = e.id;
				sTemp.tex = t;
				g_drawTextureArray.push_back(sTemp);
			}
		}
	}
	int renderer::createTexture(const char* texFileName) {
		std::string fileNameStr{ texFileName };
		std::wstring fileNameWstr{ strToWstr(fileNameStr) };
		std::wstring retFileName = fileNameWstr;
		if (fileNameStr.at(1) == ':' || fileNameStr.at(0) == '\\') {
			g_drawTextureArrayIndex++;
			g_createTextureArray[g_drawTextureArrayIndex] = fileNameStr;
			return g_drawTextureArrayIndex;
		}
		return -1;
	}
	void renderer::drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a) {
		if (g_drawTextureArray.size() > id) {
			g_drawTextureArray[id].tex.setProperties(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
		}
	}
}