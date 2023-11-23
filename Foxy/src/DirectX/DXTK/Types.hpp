#pragma once
#include "Pch/Common.hpp"
#define PI 3.141592653589793238462643383279502884L

namespace base {
	using namespace DirectX;
	class dxTexture2DWrapper {
	public:
		dxTexture2DWrapper() {
			desc.Width = 256;
			desc.Height = 256;
			desc.MipLevels = 0;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
		float imgScale(ImVec2 size, ImVec2 scale) {
			float ret{ 1.f };
			if (size.x > size.y)
				ret = size.x * scale.x;
			if (size.y > size.x)
				ret = size.y * scale.y;
			return ret;
		}
		void draw(SpriteBatch& batch) {
			ImVec2 position{ pos * ImGui::GetIO().DisplaySize };
			XMFLOAT2 xmPos{ position.x, position.y };
			XMVECTOR xmCol{ color.x, color.y, color.z, color.w };
			XMFLOAT2 origin{ center.x * desc.Width, center.x * desc.Height };
			float scale{ imgScale(ImGui::GetIO().DisplaySize / desc.Width, size) };
			float rot{ rotation * 2 * static_cast<float>(PI) };
			batch.Draw(view.Get(), xmPos, nullptr, xmCol, rot, origin, scale);
			if (GetTickCount64() > disableTime)
				enabled = false;
		}
		void setProperties(int id_, int index_, int level_, int time_, float sizeX_, float sizeY_, float centerX_, float centerY_, float posX_, float posY_, float rotation_, float screenHeightScaleFactor_, float r_, float g_, float b_, float a_) {
			enabled = true;
			id = id_;
			index = index_;
			level = level_;
			time = time_;
			size.x = sizeX_;
			size.y = sizeY_;
			center.x = centerX_;
			center.y = centerY_;
			pos.x = posX_;
			pos.y = posY_;
			rotation = rotation_;
			screenHeightScaleFactor = screenHeightScaleFactor_;
			color.x = r_;
			color.y = g_;
			color.z = b_;
			color.w = a_;
			disableTime = GetTickCount64() + time;
		}
	private:
		int id{};
		int index{};
		int level{};
		int time{};
		ImVec2 size{};
		ImVec2 center{};
		ImVec2 pos{};
		float rotation;
		float screenHeightScaleFactor;
		ImVec4 color{};
	public:
		bool enabled{};
		DWORD64 disableTime{};
		comPtr<ID3D11ShaderResourceView> view{};
		comPtr<ID3D11Resource> resource{};
		comPtr<ID3D11Texture2D> texture{};
		CD3D11_TEXTURE2D_DESC desc{};
	};
	struct scriptTex {
		dxTexture2DWrapper tex; 
		int id;
	};
	struct reloadTex {
		int id;
		std::wstring name;
	};
}