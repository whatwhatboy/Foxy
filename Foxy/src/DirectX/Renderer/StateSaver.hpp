#pragma once
#include "Pch/Common.hpp"

namespace base {
	class stateSaver {
	public:
		stateSaver(ID3D11DeviceContext* context) : m_featureLevel(D3D_FEATURE_LEVEL_11_0), m_sampleMask(0xFFFFFFF) {
			save(context);
		}
		~stateSaver() {
			release();
		}
		bool save(ID3D11DeviceContext* context) {
			if (m_savedState)
				release();
			if (!context)
				return false;
			ID3D11Device* dev{};
			context->GetDevice(&dev);
			if (!dev) {
				m_featureLevel = dev->GetFeatureLevel();
				dev->Release();
			}
			context->AddRef();
			m_context = context;
			m_context->IAGetPrimitiveTopology(&m_primitiveTopology);
			m_context->IAGetInputLayout(&m_inputLayout);
			m_context->OMGetBlendState(&m_blendState, m_blendFactor, &m_sampleMask);
			m_context->OMGetDepthStencilState(&m_depthStencilState, &m_stencilRef);
			m_context->RSGetState(&m_rasterizerState);
			m_numVertexShaderClassInstances = 256;
			m_context->VSGetShader(&m_vertexShader, m_vertexShaderClassInstances, &m_numVertexShaderClassInstances);
			m_context->VSGetConstantBuffers(0, 1, &m_vertexShaderConstantBuffer);
			m_numPixelShaderClassInstances = 256;
			m_context->PSGetShader(&m_pixelShader, m_pixelShaderClassInstances, &m_numPixelShaderClassInstances);
			m_context->PSGetShaderResources(0, 1, &m_pixelShaderResourceView);
			context->PSGetSamplers(0, 1, &m_samplerState);
			if (m_featureLevel >= D3D_FEATURE_LEVEL_10_0) {
				m_numGeometryShaderClassInstances = 256;
				m_context->GSGetShader(&m_geometryShader, m_geometryShaderClassInstances, &m_numGeometryShaderClassInstances);
				m_context->GSGetConstantBuffers(0, 1, &m_geometryShaderConstantBuffer);
				m_context->GSGetShaderResources(0, 1, &m_geometryShaderResourceView);
				if (m_featureLevel >= D3D_FEATURE_LEVEL_11_0) {
					m_numHullShaderClassInstances = 256;
					m_context->HSGetShader(&m_hullShader, m_hullShaderClassInstances, &m_numHullShaderClassInstances);
					m_numDomainShaderClassInstances = 256;
					m_context->DSGetShader(&m_domainShader, m_domainShaderClassInstances, &m_numDomainShaderClassInstances);
				}
			}
			m_context->IAGetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &m_vertexOffset);
			m_context->IAGetIndexBuffer(&m_indexBuffer, &m_indexFormat, &m_indexOffset);
			m_savedState = true;
			return true;
		}
		bool restore() {
			if (!m_savedState)
				return false;
			m_context->IASetPrimitiveTopology(m_primitiveTopology);
			m_context->IASetInputLayout(m_inputLayout);
			m_context->OMSetBlendState(m_blendState, m_blendFactor, m_sampleMask);
			m_context->OMSetDepthStencilState(m_depthStencilState, m_stencilRef);
			m_context->RSSetState(m_rasterizerState);
			m_context->VSSetShader(m_vertexShader, m_vertexShaderClassInstances, m_numVertexShaderClassInstances);
			m_context->VSSetConstantBuffers(0, 1, &m_vertexShaderConstantBuffer);
			m_context->PSSetShader(m_pixelShader, m_pixelShaderClassInstances, m_numPixelShaderClassInstances);
			m_context->PSSetShaderResources(0, 1, &m_pixelShaderResourceView);
			m_context->PSSetSamplers(0, 1, &m_samplerState);
			if (m_featureLevel >= D3D_FEATURE_LEVEL_10_0) {
				m_context->GSSetShader(m_geometryShader, m_geometryShaderClassInstances, m_numGeometryShaderClassInstances);
				m_context->GSSetConstantBuffers(0, 1, &m_geometryShaderConstantBuffer);
				m_context->GSSetShaderResources(0, 1, &m_geometryShaderResourceView);
				if (m_featureLevel >= D3D_FEATURE_LEVEL_11_0) {
					m_context->HSSetShader(m_hullShader, m_hullShaderClassInstances, m_numHullShaderClassInstances);
					m_context->DSSetShader(m_domainShader, m_domainShaderClassInstances, m_numDomainShaderClassInstances);
				}
			}
			m_context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &m_vertexOffset);
			m_context->IASetIndexBuffer(m_indexBuffer, m_indexFormat, m_indexOffset);
			return true;
		}
		template <typename T>
		void releaseObject(T*& p) {
			if (p) {
				p->Release();
				p = nullptr;
			}
		}
		void release() {
			m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
			releaseObject(m_inputLayout);
			releaseObject(m_blendState);
			for (int i{}; i != 4; ++i)
				m_blendFactor[i] = 0.f;
			m_sampleMask = 0xFFFFFFFF;
			releaseObject(m_depthStencilState);
			m_stencilRef = 0;
			releaseObject(m_rasterizerState);
			releaseObject(m_pixelShaderResourceView);
			releaseObject(m_samplerState);
			releaseObject(m_vertexShader);
			for (UINT i{}; i != m_numVertexShaderClassInstances; ++i)
				releaseObject(m_vertexShaderClassInstances[i]);
			m_numVertexShaderClassInstances = 0;
			releaseObject(m_vertexShaderConstantBuffer);
			releaseObject(m_geometryShader);
			for (UINT i{}; i != m_numGeometryShaderClassInstances; ++i)
				releaseObject(m_geometryShaderClassInstances[i]);
			m_numGeometryShaderClassInstances = 0;
			releaseObject(m_geometryShaderConstantBuffer);
			releaseObject(m_geometryShaderResourceView);
			releaseObject(m_pixelShader);
			for (UINT i{}; i != m_numPixelShaderClassInstances; ++i)
				releaseObject(m_pixelShaderClassInstances[i]);
			m_numPixelShaderClassInstances = 0;
			releaseObject(m_hullShader);
			for (UINT i{}; i != m_numHullShaderClassInstances; ++i)
				releaseObject(m_hullShaderClassInstances[i]);
			m_numHullShaderClassInstances = 0;
			releaseObject(m_domainShader);
			for (UINT i{}; i != m_numDomainShaderClassInstances; ++i)
				releaseObject(m_domainShaderClassInstances[i]);
			m_numDomainShaderClassInstances = 0;
			releaseObject(m_vertexBuffer);
			m_vertexStride = 0;
			m_vertexOffset = 0;
			releaseObject(m_indexBuffer);
			m_indexFormat = DXGI_FORMAT_UNKNOWN;
			m_indexOffset = 0;
			releaseObject(m_context);
			m_featureLevel = D3D_FEATURE_LEVEL_11_0;
			m_savedState = false;
		}
	private:
		bool m_savedState{};
		D3D_FEATURE_LEVEL m_featureLevel{};
		ID3D11DeviceContext* m_context{};
		D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology{};
		ID3D11InputLayout* m_inputLayout{};
		ID3D11BlendState* m_blendState{};
		FLOAT m_blendFactor[4]{};
		UINT m_sampleMask{};
		ID3D11DepthStencilState* m_depthStencilState{};
		UINT m_stencilRef{};
		ID3D11RasterizerState* m_rasterizerState{};
		ID3D11ShaderResourceView* m_pixelShaderResourceView{};
		ID3D11SamplerState* m_samplerState{};
		ID3D11VertexShader* m_vertexShader{};
		ID3D11ClassInstance* m_vertexShaderClassInstances[256]{};
		UINT m_numVertexShaderClassInstances{};
		ID3D11Buffer* m_vertexShaderConstantBuffer{};
		ID3D11GeometryShader* m_geometryShader{};
		ID3D11ClassInstance* m_geometryShaderClassInstances[256]{};
		UINT m_numGeometryShaderClassInstances{};
		ID3D11Buffer* m_geometryShaderConstantBuffer{};
		ID3D11ShaderResourceView* m_geometryShaderResourceView{};
		ID3D11PixelShader* m_pixelShader{};
		ID3D11ClassInstance* m_pixelShaderClassInstances[256]{};
		UINT m_numPixelShaderClassInstances{};
		ID3D11HullShader* m_hullShader{};
		ID3D11ClassInstance* m_hullShaderClassInstances[256]{};
		UINT m_numHullShaderClassInstances{};
		ID3D11DomainShader* m_domainShader{};
		ID3D11ClassInstance* m_domainShaderClassInstances[256]{};
		UINT m_numDomainShaderClassInstances{};
		ID3D11Buffer* m_vertexBuffer{};
		UINT m_vertexStride{};
		UINT m_vertexOffset{};
		ID3D11Buffer* m_indexBuffer{};
		DXGI_FORMAT m_indexFormat{};
		UINT m_indexOffset{};
	};
}