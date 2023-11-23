#pragma once
#include "Pch/Common.hpp"
#include "Core/Logger.hpp"
#include "DirectX/Renderer/StateSaver.hpp"
#define RETURN(...) return { __VA_ARGS__ };
#define RETURN_DEFAULT return {};
#define IS_VALID(check) \
	if (!check) { \
		RETURN_DEFAULT; \
	}
#define RETURN_IF_VALID(check, ...) IS_VALID(check) RETURN(check, __VA_ARGS__)

union vectorCordValue {
	int16_t i16;
	uint16_t u16;
	int32_t i32;
	uint32_t u32;
	int32_t i64;
	uint32_t u64;
	float fPoint;
};
struct vec2 {
	vectorCordValue x{}, y{};
};
struct image { unsigned char* data; vec2 size; };
struct imageData { unsigned char* bytes; int size; uint32_t delay; };
struct shaderData { ID3D11Resource* resource{}; ID3D11ShaderResourceView* resourceView{}; };
struct frameData { uint32_t delay; shaderData shader; };
struct gif { unsigned char* bytes; imageData data; int* delays; int frameCount; vec2 size; int comp; };
struct Rect {
	float left, right{};
	float top, bottom{};
	ImVec2 size{};
	void get(RECT& handle) {
		LOG_DEBUG("L{}, R{}, T{}, B{} S({}, {})", left, right, top, bottom, size.x, size.y)
		left = handle.left;
		right = handle.right;
		top = handle.top;
		bottom = handle.bottom;
		size.x = right - left;
		size.y = bottom - top;
	}
};
namespace base {
	namespace imgui {
		extern void setColors();
		extern void createHandles(comPtr<ID3D11Device> dev, comPtr<ID3D11DeviceContext> ctx);
		extern void createFont(ImFontAtlas* atlas, ImFont*& font, fs::path path, float size, ImFontConfig* cfg);
		template <size_t dataSize>
		extern void createFont(ImFontAtlas* atlas, ImFont*& font, uint8_t(&data)[dataSize], float size, ImFontConfig* cfg);
	}
	namespace stb {
		namespace memory {
			extern imageData write(vec2& size, int comp, const void* data, int strideBytes, uint32_t delay);
			extern image read(imageData data);
			extern gif readGif(imageData data);
		}
		namespace file {
			extern image readImage(fs::path path);
			extern gif readGif(fs::path path);
		}
		extern std::vector<imageData> splitGif(fs::path path);
	}
	class rendererQueue {
	public:
		void tick() {
			for (auto& fn : m_funcs)
				fn.second();
		}
		void add(std::string id, std::function<void()> func) {
			if (m_funcs.size() == NULL)
				m_funcs.insert({ id, std::move(func) });
		}
		void remove(std::string id) {
			m_funcs.erase(id);
		}
	public:
		std::map<std::string, std::function<void()>> m_funcs;
	};
	inline rendererQueue g_rendererQueue;
	inline std::string wstrToStr(std::wstring str) {
		return fs::path(str).string();
	}
	inline std::wstring strToWstr(std::string str) {
		return fs::path(str).wstring();
	}
	class renderer {
	public:
		renderer();
		~renderer();
	public:
		void present();
		static LRESULT wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	public:
		std::pair<shaderData, vec2> createTexture(fs::path path);
		std::vector<frameData> createGifTexture(fs::path path);
		shaderData createShaderData(image image);
	public:
		//SHV fuckery
		void createTextures();
		void reloadTextures();
		int createTexture(const char* texFileName);
		void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a);
	public:
		Rect m_window{};
		int m_headerFrame{};
		std::vector<frameData> m_header{};
	public:
		ImFontConfig m_fontCfg{};
		ImFont* m_font{};
		ImFont* m_breakFont{};
		ImFont* m_openTooltipFont{};
		ImFont* m_notifyFont{};
		ImFont* m_headerFont{};
		ImFont* m_subtitleFont{};
		ImFont* m_optionFont{};
		ImFont* m_smallIconFont{};
		ImFont* m_iconFont{};
		ImFont* m_bigIconFont{};
		ImFont* m_footerFont{};
		ImFont* m_infoboxFont{};
		ImFont* m_playerinfoFont{};
	public:
		comPtr<IDXGISwapChain> m_swapchain{};
		comPtr<ID3D11Device> m_device{};
		comPtr<ID3D11DeviceContext> m_context{};
		bool m_stateSaved{};
		std::unique_ptr<stateSaver> m_stateSaver{};
		std::unique_ptr<DirectX::CommonStates> m_commonState{};
		std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch{};
	public:
		WNDPROC m_wndProc{};
	};
	inline std::unique_ptr<renderer> g_renderer{};
}