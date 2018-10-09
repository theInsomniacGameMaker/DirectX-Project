#pragma once
#include "Declarations.h"

class TextureRenderer
{
public:
	TextureRenderer();
	TextureRenderer(ID3D11Device *pDevice, int width, int height);
	~TextureRenderer();
	ID3D11ShaderResourceView *finalTexture;
	ID3D11ShaderResourceView *pResView;

private:
	ID3D11RenderTargetView *renderTargetView;
	ID3D11Texture2D *texture;
	ID3D11RenderTargetView *renderTargetView;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11DepthStencilView* mainDepthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
	XMVECTOR rttEye;
	XMVECTOR rttAt ;
	XMVECTOR rttUp ;

public:
	void BeginRender(ID3D11DeviceContext* pDeviceContext);
	void EndRender(ID3D11DeviceContext *pDeviceContext);
	void Clear(ID3D11DeviceContext *pDeviceContext, ID3D11DepthStencilView *pDepth,
		XMFLOAT4 clearColor);
	void MoveCamera(ConstantBuffer & cb, ID3D11Buffer *& constantBuffer, ID3D11DeviceContext *& pDeviceContext);
	ID3D11ShaderResourceView* GetTexture() { return finalTexture; }
};

