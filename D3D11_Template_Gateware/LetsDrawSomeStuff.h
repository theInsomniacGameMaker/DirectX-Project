// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

#pragma comment (lib, "d3d11.lib")

// Include DirectX11 for interface access
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <iostream>
#include <time.h>

using namespace DirectX;
using namespace std;

#include "Trivial_VS.csh"
#include "Trivial_PS.csh"

// BEGIN PART 1
// TODO: PART 1 STEP 1a

// TODO: PART 1 STEP 1b


struct SIMPLE_VERTEX
{
	XMFLOAT2 point;
};

// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device *myDevice = nullptr;
	IDXGISwapChain *mySwapChain = nullptr;
	ID3D11DeviceContext *myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	ID3D11RenderTargetView* myRenderTargetView = nullptr;
	D3D11_VIEWPORT viewport;
	ID3D11Buffer * myVertexBuffer;
	ID3D11VertexShader*     myVertexShader = nullptr;
	ID3D11PixelShader*      myPixelShader = nullptr;

	ID3D11InputLayout*      myVertexLayout = nullptr;

public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			ID3D11Texture2D* myBackBuffer = nullptr;
			mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBuffer));
			myDevice->CreateRenderTargetView(myBackBuffer, nullptr, &myRenderTargetView);

			// TODO: PART 1 STEP 5
			unsigned int width, height; 
			attatchPoint->GetClientWidth(width);
			attatchPoint->GetClientHeight(height);
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = width;
			viewport.Height = height;
			viewport.MinDepth = 0;
			viewport.MaxDepth = 1;

			SIMPLE_VERTEX circleVerts[361];
			for (int i = 0; i < 361; i++)
			{
				circleVerts[i].point.x = sin(i*3.14f / 180.0f);
				circleVerts[i].point.y = cos(i*3.14f / 180.0f);
			}


			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SIMPLE_VERTEX) * 361;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;


			D3D11_SUBRESOURCE_DATA InitData = {};
			InitData.pSysMem = circleVerts;

			myDevice->CreateBuffer(&bd, &InitData, &myVertexBuffer);

			myDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), nullptr, &myVertexShader);
			myDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &myPixelShader);

			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			myDevice->CreateInputLayout(layout, numElements, Trivial_VS,
				sizeof(Trivial_VS), &myVertexLayout);

		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	// Release DX Objects aquired from the surface
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();

	// TODO: "Release()" more stuff here!


	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		// this could be changed during resolution edits, get it every frame
		//ID3D11RenderTargetView *myRenderTargetView = nullptr;
		ID3D11DepthStencilView *myDepthStencilView = nullptr;
		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
				myDepthStencilView->Release();
			}

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// Clear the screen to dark green
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);
			
			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			UINT stride = sizeof(SIMPLE_VERTEX);
			UINT offset = 0;
			myContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);

			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->PSSetShader(myPixelShader, nullptr, 0);

			myContext->IASetInputLayout(myVertexLayout);

			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

			myContext->Draw(361, 0);

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}