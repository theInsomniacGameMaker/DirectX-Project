#pragma once
#include "Declarations.h"

class TextureRenderer
{
public:
	TextureRenderer();
	TextureRenderer(ID3D11Device *pDevice, int width, int height);
	~TextureRenderer();
	ID3D11ShaderResourceView *pCTexture;
	ID3D11ShaderResourceView *pResView;

private:
	ID3D11RenderTargetView *pRenderTargetView;
	ID3D11Texture2D *pTexture;
	ID3D11RenderTargetView *pTarget;

public:
	void BeginRender(ID3D11DeviceContext* pDeviceContext);
	void EndRender(ID3D11DeviceContext *pDeviceContext);
	void Clear(ID3D11DeviceContext *pDeviceContext, ID3D11DepthStencilView *pDepth,
		XMFLOAT4 clearColor);

	ID3D11ShaderResourceView* GetTexture() { return pCTexture; }
};
