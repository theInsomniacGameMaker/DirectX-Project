#include "TextureRenderer.h"

TextureRenderer::TextureRenderer(ID3D11Device *pDevice, int width, int height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT hr = pDevice->CreateTexture2D(&textureDesc, nullptr, &pTexture);

	// Setup the description of the render target view.
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = pDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
	

	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = pDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pResView);
	
	rttEye = XMVectorSet(100, 0, -3, 0);
	rttAt = XMVectorSet(100, 0, 0, 0);
	rttUp = XMVectorSet(0, 1, 0, 0);

	pCTexture =  (pResView);
}

TextureRenderer::TextureRenderer()
{

}

TextureRenderer::~TextureRenderer()
{
	if (pCTexture)
	{
		delete pCTexture;
		pCTexture = nullptr;
	}

	if (pResView)
	{
		pResView->Release();
		pResView = 0;
	}

	if (pRenderTargetView)
	{
		pRenderTargetView->Release();
		pRenderTargetView = 0;
	}

	if (pTexture)
	{
		pTexture->Release();
		pTexture = 0;
	}
}

void TextureRenderer::Clear(ID3D11DeviceContext *pDeviceContext, ID3D11DepthStencilView *pDepth,
	XMFLOAT4 clearColor)
{
	float color[] = {
		clearColor.x, clearColor.y, clearColor.z, clearColor.w
	};

	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
	
}

void TextureRenderer::MoveCamera(ConstantBuffer &cb, ID3D11Buffer *& constantBuffer, ID3D11DeviceContext *&pDeviceContext)
{
	cb.mView = XMMatrixTranspose(XMMatrixLookAtLH(rttEye, rttAt, rttUp));
	pDeviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
}

void TextureRenderer::BeginRender(ID3D11DeviceContext *pDeviceContext)
{
	ID3D11DepthStencilView *pDepthStencil = nullptr;
	pDeviceContext->OMGetRenderTargets(1, &pTarget, &pDepthStencil);

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencil);
}

void TextureRenderer::EndRender(ID3D11DeviceContext *pDeviceContext)
{
	ID3D11DepthStencilView *pDepthStencil = nullptr;
	pDeviceContext->OMGetRenderTargets(0, nullptr, &pDepthStencil);

	pDeviceContext->OMSetRenderTargets(1, &pTarget, pDepthStencil);
}


