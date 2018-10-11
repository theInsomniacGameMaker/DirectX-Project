#pragma once
#include "Declarations.h"

class TextureRenderer
{
public:
	TextureRenderer();
	TextureRenderer(CComPtr<ID3D11Device> pDevice, int width, int height);
	~TextureRenderer();
	CComPtr<ID3D11ShaderResourceView> pCTexture;
	CComPtr<ID3D11ShaderResourceView>pResView;
	CComPtr<ID3D11RenderTargetView>pRenderTargetView;

private:
	CComPtr<ID3D11ShaderResourceView>nullResView = nullptr;
	CComPtr<ID3D11Texture2D >pTexture;
	CComPtr<ID3D11RenderTargetView >pTarget;
	CComPtr<ID3D11DepthStencilView> depthStencilView;
	CComPtr<ID3D11Texture2D> depthStencilBuffer;
	XMVECTOR rttEye;
	XMVECTOR rttAt;
	XMVECTOR rttUp;

public:
	void BeginRender(CComPtr < ID3D11DeviceContext> pDeviceContext, CComPtr<ID3D11RenderTargetView> &immediateRTV);
	void EndRender(CComPtr < ID3D11DeviceContext > pDeviceContext, CComPtr < ID3D11RenderTargetView > mainRenderTargetView, CComPtr < ID3D11DepthStencilView > mainDepthStencilView);
	void ClearDPV(CComPtr<ID3D11DeviceContext> pDeviceContext);
	void Clear(CComPtr < ID3D11DeviceContext> pDeviceContext, CComPtr < ID3D11DepthStencilView >pDepth, XMFLOAT4 clearColor);
	void MoveCamera(ConstantBuffer & cb, CComPtr < ID3D11Buffer >& constantBuffer, CComPtr < ID3D11DeviceContext >& pDeviceContext);
	ID3D11ShaderResourceView* GetTexture() { return pCTexture; }
};

