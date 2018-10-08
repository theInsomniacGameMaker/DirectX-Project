#include "TextureRenderer.h"

TextureRenderer::TextureRenderer(ID3D11Device *pDevice, int width, int height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;


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
	//if (FAILED(hr))
		//throw directx_init_error(hr, "Texture2D");

	// Setup the description of the render target view.
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	hr = pDevice->CreateRenderTargetView(pTexture, &renderTargetViewDesc, &pRenderTargetView);
	//if (FAILED(hr))
		//throw(directx_init_error(hr, "RTV"));

	ZeroMemory(&shaderResourceViewDesc, sizeof(shaderResourceViewDesc));
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	hr = pDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pResView);
	//if (FAILED(hr))
		//throw directx_init_error(hr, "shader resource view");
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
	if (pDepth)
		pDeviceContext->ClearDepthStencilView(pDepth, D3D11_CLEAR_DEPTH, 1.0f, 0);
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

