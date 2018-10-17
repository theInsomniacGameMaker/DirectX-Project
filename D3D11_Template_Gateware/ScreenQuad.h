#pragma once
#include "Declarations.h"
class ScreenQuad
{
	UINT stride[1];
	UINT offset[1];
	SimpleVertex* screenQuadVertices;
	CComPtr<ID3D11Device> m_Device = nullptr;
	CComPtr < ID3D11DeviceContext> m_Context = nullptr;
	CComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;
	CComPtr < ID3D11Buffer> m_IndexBuffer = nullptr;
	CComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
	CComPtr < ID3D11PixelShader> m_PixelShader = nullptr;
	CComPtr<ID3D11GeometryShader> m_GeometryShader = nullptr;
	CComPtr < ID3D11ShaderResourceView> m_TextureRV = nullptr;

public:
	ScreenQuad();
	ScreenQuad(CComPtr < ID3D11Device >& myDevice, CComPtr < ID3D11DeviceContext >& myContext,
		CComPtr < ID3D11VertexShader >& vertexShader, CComPtr < ID3D11PixelShader>& pixelShader,
		CComPtr < ID3D11GeometryShader> &geoShader, XMFLOAT3 positions[]);
	void Render();
	void UpdateTexture(CComPtr < ID3D11ShaderResourceView> &textureRV);
	void UpdatePS(CComPtr < ID3D11PixelShader> pixelShader);
	~ScreenQuad();
};

