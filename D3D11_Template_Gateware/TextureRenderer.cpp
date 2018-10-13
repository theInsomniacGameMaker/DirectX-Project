#include "TextureRenderer.h"

TextureRenderer::TextureRenderer(CComPtr<ID3D11Device> pDevice, int width, int height)
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

	pDevice->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);
	pDevice->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

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

	hr = pDevice->CreateShaderResourceView(pTexture, &shaderResourceViewDesc, &pCTexture);
	
	rttEye = XMVectorSet(100, 0, -3, 0);
	rttAt = XMVectorSet(100, 0, 0, 0);
	rttUp = XMVectorSet(0, 1, 0, 0);

	//pCTexture =  (pResView);
}

TextureRenderer::TextureRenderer()
{

}

TextureRenderer::~TextureRenderer()
{
	//if (pCTexture)
	//{
	//	//pCTexture->Release();
	//	pCTexture = 0;
	//}
	/*
	ID3D11RenderTargetView *pRenderTargetView;
	ID3D11Texture2D *pTexture;
	ID3D11RenderTargetView *pTarget;
	ID3D11DepthStencilView* depthStencilView;
	ID3D11DepthStencilView* mainDepthStencilView;
	ID3D11Texture2D* depthStencilBuffer;*/

	/*pRenderTargetView->Release();
	pTexture->Release();
	depthStencilView->Release();
	depthStencilBuffer->Release();*/
	//mainDepthStencilView->Release();
	//pTarget->Release();

}

void TextureRenderer::Clear(CComPtr < ID3D11DeviceContext> pDeviceContext, CComPtr < ID3D11DepthStencilView >pDepth, XMFLOAT4 clearColor)
{
	float color[] = {
		clearColor.x, clearColor.y, clearColor.z, clearColor.w
	};

	pDeviceContext->ClearRenderTargetView(pRenderTargetView, color);
	pDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
}

void TextureRenderer::MoveCamera(ConstantBuffer & cb, CComPtr < ID3D11Buffer >& constantBuffer, CComPtr < ID3D11DeviceContext >& pDeviceContext)
{
	cb.mView = XMMatrixTranspose(XMMatrixLookAtLH(rttEye, rttAt, rttUp));
	pDeviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);
}

void TextureRenderer::BeginRender(CComPtr <ID3D11DeviceContext> pDeviceContext, CComPtr<ID3D11RenderTargetView> &immediateRTV)
{
	pDeviceContext->OMGetRenderTargets(1, nullptr, nullptr);
	//pTarget.Release();
	pTarget = immediateRTV;
	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView.p, depthStencilView);
}

void TextureRenderer::EndRender(CComPtr<ID3D11DeviceContext> pDeviceContext, CComPtr<ID3D11RenderTargetView> mainRenderTargetView, CComPtr<ID3D11DepthStencilView> mainDepthStencilView)
{
	//ID3D11DepthStencilView *pDepthStencil = nullptr;
	pDeviceContext->OMGetRenderTargets(0, nullptr, nullptr);
	pDeviceContext->OMSetRenderTargets(1, &pTarget.p, mainDepthStencilView);
	//pResView = {nullptr};
	//pDeviceContext->PSSetShaderResources(0, 1, &pResView.p);

}

void TextureRenderer::ClearDPV(CComPtr < ID3D11DeviceContext> pDeviceContext)
{
	pDeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
}


