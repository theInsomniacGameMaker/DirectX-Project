// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include "Declarations.h"
#include "Mesh.h"
#include "Misc.h"
#include "XTime.h"
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
	D3D_DRIVER_TYPE				myDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			myFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11RenderTargetView*		myRenderTargetView = nullptr;
	ID3D11VertexShader*			myVertexShader = nullptr;
	ID3D11VertexShader*			myVertexShaderUV = nullptr;
	ID3D11PixelShader*			myPixelShader = nullptr;
	ID3D11InputLayout*			myVertexLayout = nullptr;

#if CHARIZARD_MESH
	ID3D11Buffer*				charizardVertexBuffer = nullptr;
	ID3D11Buffer*				charizardIndexBuffer = nullptr;
#endif

#if BOX_MESH
	ID3D11Buffer*				boxVertexBuffer = nullptr;
	ID3D11Buffer*				boxIndexBuffer = nullptr;
	ID3D11ShaderResourceView*   myTextureRVBox = nullptr;
#endif

#if SPACESHIP
	ID3D11Buffer*				spaceshipVertexBuffer = nullptr;
	ID3D11Buffer*				spaceshipIndexBuffer = nullptr;
#endif

#if PROCEDURAL_SPHERE
	ID3D11Buffer*				sphereVertexBuffer = nullptr;
	ID3D11Buffer*				sphereIndexBuffer = nullptr;
#endif

	ID3D11Buffer*				bulbVertexBuffer = nullptr;
	ID3D11Buffer*				bulbIndexBuffer = nullptr;

	ID3D11Buffer*				groundVertexBuffer = nullptr;
	ID3D11Buffer*				groundIndexBuffer = nullptr;

	ID3D11Buffer*				myConstantBuffer = nullptr;
	XMMATRIX					worldMatrix;
	XMMATRIX					viewMatrix;
	XMMATRIX					projectionMatrix;
	ID3D11ShaderResourceView*   myTextureRVSpaceShip = nullptr;
	ID3D11ShaderResourceView*   myTextureRVBulb = nullptr;
	ID3D11ShaderResourceView*   myTextureRVBase = nullptr;
	ID3D11SamplerState*			mySamplerLinear = nullptr;

	//IDirectInputDevice8* DIKeyboard = nullptr;
	//IDirectInputDevice8* DIMouse = nullptr;

	//DIMOUSESTATE mouseLastState;
	//LPDIRECTINPUT8 DirectInput;

	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

#if WIREFRAME
	ID3D11RasterizerState* WireFrame;
#endif
	XTime timer;
	Mesh charizard;
	Mesh box;
	Mesh ground;
	Mesh bulb;
	Mesh spaceShip;
public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
	//Camera Movement
	void CameraMovement();

	bool InitDirectInput(HINSTANCE hInstance);
};

// Init
LetsDrawSomeStuff::LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint)
{
	//winClass.hInstance = GetModuleHandleW(0);

	if (attatchPoint) // valid window?
	{
		// Create surface, will auto attatch to GWindow
		if (G_SUCCESS(GW::GRAPHICS::CreateGDirectX11Surface(attatchPoint, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT, &mySurface)))
		{
			// Grab handles to all DX11 base interfaces
			mySurface->GetDevice((void**)&myDevice);
			mySurface->GetSwapchain((void**)&mySwapChain);
			mySurface->GetContext((void**)&myContext);

			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)
			ID3D11Texture2D* myBackBuffer = nullptr;
			mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBuffer));

			myDevice->CreateRenderTargetView(myBackBuffer, nullptr, &myRenderTargetView);
			myBackBuffer->Release();

			myContext->OMSetRenderTargets(1, &myRenderTargetView, nullptr);

			D3D11_VIEWPORT vp;
			unsigned int width, height;
			attatchPoint->GetClientWidth(width);
			attatchPoint->GetClientHeight(height);
			vp.Width = (FLOAT)width;
			vp.Height = (FLOAT)height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			myContext->RSSetViewports(1, &vp);

			HRESULT hr = myDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), nullptr, &myVertexShader);
			hr = myDevice->CreateVertexShader(VS_UVModifier, sizeof(VS_UVModifier), nullptr, &myVertexShaderUV);
			myDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &myPixelShader);

			// Define the input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORDP", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			UINT numElements = ARRAYSIZE(layout);

			// Create the input layout
			myDevice->CreateInputLayout(layout, numElements, Trivial_VS, sizeof(Trivial_VS), &myVertexLayout);

			myContext->IASetInputLayout(myVertexLayout);

			D3D11_BUFFER_DESC bd = {};
			D3D11_SUBRESOURCE_DATA InitData = {};
			UINT stride[] = { sizeof(SimpleVertex) };
			UINT offset[] = { 0 };

#if WIREFRAME 
			D3D11_RASTERIZER_DESC wfdesc;
			ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
			wfdesc.FillMode = D3D11_FILL_WIREFRAME;
			wfdesc.CullMode = D3D11_CULL_NONE;
			myDevice->CreateRasterizerState(&wfdesc, &WireFrame);
#endif

#if CHARIZARD_MESH
			charizard = Mesh("Charizard.fbx", 25.0f, myDevice, myTextureRV);
#endif

#if PROCEDURAL_SPHERE
			CreateSphere(10, 10, myDevice, sphereVertexBuffer, sphereIndexBuffer);
#endif 

#if BOX_MESH
			box = Mesh("cube.fbx", 1 / 50.f, myDevice, myTextureRVBox);
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) *box.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData = {};
			InitData.pSysMem = box.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &boxVertexBuffer);

			//myContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, stride, offset);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * box.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = box.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &boxIndexBuffer);
			// Set index buffer
			//myContext->IASetIndexBuffer(boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			//myContext->DrawIndexed(box.GetNumberOfIndices(), 0, 0);
#endif

#if SPACESHIP
			spaceShip = Mesh("Galaga Fighter.fbx", 1 / 4.0f, myDevice, myTextureRVSpaceShip);

			//Setting buffer description
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) *spaceShip.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData = {};
			InitData.pSysMem = spaceShip.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &spaceshipVertexBuffer);

			//myContext->IASetVertexBuffers(0, 1, &spaceshipVertexBuffer, stride, offset);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * spaceShip.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = spaceShip.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &spaceshipIndexBuffer);
			// Set index buffer
			//myContext->IASetIndexBuffer(spaceshipIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			//myContext->DrawIndexed(spaceShip.GetNumberOfIndices(), 0, 0);
/*
			spaceshipVertexBuffer->Release();
			spaceshipIndexBuffer->Release();*/
#endif 

#pragma region BULB_INIT
			bulb = Mesh("Bulb.fbx", 1.0f / 5, myDevice, myTextureRVBulb);
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) *bulb.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData = {};
			InitData.pSysMem = bulb.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &bulbVertexBuffer);

			//myContext->IASetVertexBuffers(0, 1, &bulbVertexBuffer, stride, offset);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * bulb.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = bulb.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &bulbIndexBuffer);

			// Set index buffer
			//myContext->IASetIndexBuffer(bulbIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			//myContext->DrawIndexed(bulb.GetNumberOfIndices(), 0, 0);

#pragma endregion

#pragma region GROUND_INIT
			ground = Mesh("Ground.fbx", 10, myDevice, myTextureRVBase);

			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) *ground.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData = {};
			InitData.pSysMem = ground.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &groundVertexBuffer);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * ground.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = ground.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &groundIndexBuffer);
#pragma endregion


			// Set primitive topology
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

			// Create the constant buffer
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			myDevice->CreateBuffer(&bd, nullptr, &myConstantBuffer);

			D3D11_SAMPLER_DESC sampDesc = {};
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			myDevice->CreateSamplerState(&sampDesc, &mySamplerLinear);

			// Initialize the world matrix
			worldMatrix = XMMatrixIdentity();

			// Initialize the view matrix
			Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
			At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

			// Initialize the projection matrix
			projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);

			timer.Restart();
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
	myVertexShader->Release();
	myVertexShaderUV->Release();
	myPixelShader->Release();
	myVertexLayout->Release();

#if CHARIZARD_MESH
	if (charizardVertexBuffer)charizardVertexBuffer->Release();
	if (charizardIndexBuffer)charizardIndexBuffer->Release();
#endif

#if BOX_MESH
	if (boxVertexBuffer)boxVertexBuffer->Release();
	if (boxIndexBuffer)boxIndexBuffer->Release();
	if (myTextureRVBox)myTextureRVBox->Release();
#endif

#if PROCEDURAL_SPHERE
	if (sphereVertexBuffer)sphereVertexBuffer->Release();
	if (sphereVertexBuffer)sphereIndexBuffer->Release();
#endif

	groundVertexBuffer->Release();
	groundIndexBuffer->Release();
	if (myTextureRVBase)myTextureRVBase->Release();

	bulbVertexBuffer->Release();
	bulbIndexBuffer->Release();
	if (myTextureRVBulb)myTextureRVBulb->Release();

	myConstantBuffer->Release();

	spaceshipVertexBuffer->Release();
	spaceshipIndexBuffer->Release();
	if (myTextureRVSpaceShip)myTextureRVSpaceShip->Release();

	if (mySamplerLinear)mySamplerLinear->Release();


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
		timer.Signal();
		// this could be changed during resolution edits, get it every frame
		/*ID3D11RenderTargetView *myRenderTargetView = nullptr;*/
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
			const float d_green[] = { 0, 0.0f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);

			viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

#if WIREFRAME
			myContext->RSSetState(WireFrame);
#endif

			// TODO: Set your shaders, Update & Set your constant buffers, Attatch your vertex & index buffers, Set your InputLayout & Topology & Draw!
			// Update our time
			static float t = 0.0f;
			if (myDriverType == D3D_DRIVER_TYPE_REFERENCE)
			{
				t += (float)XM_PI * 0.0125f;
			}
			else
			{
				static ULONGLONG timeStart = 0;
				ULONGLONG timeCur = GetTickCount64();
				if (timeStart == 0)
					timeStart = timeCur;
				t = (timeCur - timeStart) / 1000.0f;
			}

			CameraMovement();

			//animate cube
			worldMatrix = XMMatrixRotationY(t);

			XMFLOAT4 vLightDirs[2] =
			{
				XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f),
			};
			XMFLOAT4 vLightColors[2] =
			{
			#if DIRECTIONAL_LIGHT_ON
				XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
			#else
				XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
				XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
			#endif
			};

			// Rotate the a matrix
			XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
			//store the ligh dir in a XMFloat 
			XMVECTOR vLightDir = XMLoadFloat4(&vLightDirs[1]);
			//transform the light dir by the roatation matrix made
			vLightDir = XMVector3Transform(vLightDir, mRotate);
			//copy the values back
			XMStoreFloat4(&vLightDirs[1], vLightDir);

			ConstantBuffer cb;
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			cb.mView = XMMatrixTranspose(viewMatrix);
			cb.mProjection = XMMatrixTranspose(projectionMatrix);
			cb.vLightDir[0] = vLightDirs[0];
			cb.vLightDir[1] = vLightDirs[1];
			cb.vLightColor[0] = vLightColors[0];
			cb.vLightColor[1] = vLightColors[1];
			cb.pointLight.pos = XMFLOAT4(0, (sin((float)timer.TotalTime()) * 5), 0, 0);
			cb.pointLight.range = 6.0f;
			cb.pointLight.diffuse = XMFLOAT4(0, 0, 1, 1);
			cb.time = t;
			cb.vOutputColor = XMFLOAT4(0, 0, 0, 0);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			//
			// Render the cube
			//
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShaderResources(0, 1, &myTextureRVSpaceShip);
			myContext->PSSetSamplers(0, 1, &mySamplerLinear);


			//Setting buffer description
			D3D11_BUFFER_DESC bd = {};
			D3D11_SUBRESOURCE_DATA InitData = {};
			UINT stride[] = { sizeof(SimpleVertex) };
			UINT offset[] = { 0 };
#if CHARIZARD_MESH
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * charizard.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData.pSysMem = charizard.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &charizardVertexBuffer);

			// Set vertex buffer
			myContext->IASetVertexBuffers(0, 1, &charizardVertexBuffer, stride, offset);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * charizard.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = charizard.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &charizardIndexBuffer);
			// Set index buffer
			myContext->IASetIndexBuffer(charizardIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myContext->DrawIndexed(charizard.GetNumberOfIndices(), 0, 0);

			if (charizardVertexBuffer)charizardVertexBuffer->Release();
			if (charizardIndexBuffer)charizardIndexBuffer->Release();
#endif

#if BOX_MESH
			myContext->VSSetShader(myVertexShaderUV, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBox);

			XMVECTOR boxPosition = { -3, 0.0f,0,0 };
			//XMMatrixTranslationFromVector(boxPosition);
			cb.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(boxPosition));
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(box.GetNumberOfIndices(), 0, 0);
#endif

#if SPACESHIP
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &myTextureRVSpaceShip);

			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &spaceshipVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(spaceshipIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(spaceShip.GetNumberOfIndices(), 0, 0);

			/*spaceshipVertexBuffer->Release();
			spaceshipIndexBuffer->Release();*/
#endif 
#if PROCEDURAL_SPHERE
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * numSphereVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			InitData = {};
			InitData.pSysMem = &sphereVertices[0];
			myDevice->CreateBuffer(&bd, &InitData, &sphereVertexBuffer);

			myContext->IASetVertexBuffers(0, 1, &sphereVertexBuffer, stride, offset);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * numSphereIndices;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = &indices[0];
			myDevice->CreateBuffer(&bd, &InitData, &sphereIndexBuffer);
			// Set index buffer
			myContext->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(sphereFaces * 3, 0, 0);
			sphereVertexBuffer->Release();
			sphereIndexBuffer->Release();
#endif

#pragma region BULB_RENDER
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBulb);
			myContext->PSSetSamplers(0, 1, &mySamplerLinear);

			worldMatrix = XMMatrixTranslationFromVector(XMVECTOR{ cb.pointLight.pos.x, cb.pointLight.pos.y, cb.pointLight.pos.z, cb.pointLight.pos.w });
			cb.mWorld = XMMatrixTranspose(worldMatrix);

			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);


			myContext->IASetVertexBuffers(0, 1, &bulbVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(bulbIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(bulb.GetNumberOfIndices(), 0, 0);
#pragma endregion


#pragma region GROUND_RENDER			
			//Rendering ground
			XMVECTOR groundPosition = { 0,-0.5f,0,0 };
			worldMatrix = XMMatrixTranslationFromVector(groundPosition);
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBase);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);


			myContext->IASetVertexBuffers(0, 1, &groundVertexBuffer, stride, offset);
			// Set index buffer
			myContext->IASetIndexBuffer(groundIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(ground.GetNumberOfIndices(), 0, 0);
#pragma endregion



			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			myRenderTargetView->Release(); // Free any temp DX handles aquired this frame
		}
	}
}

void LetsDrawSomeStuff::CameraMovement()
{
	XMFLOAT4 right;
	XMStoreFloat4(&right, At);
	XMVECTOR rightVector = { right.z, right.y,-right.x };
	rightVector = XMVector3Normalize(rightVector);
	if (GetAsyncKeyState('E'))
	{
		Eye += (UP*(float)timer.Delta() * 5.0f);
		At += (UP*(float)timer.Delta() * 5.0f);
		}
	else if (GetAsyncKeyState('Q'))
	{
		Eye -= (UP*(float)timer.Delta() * 5.0f);
		At -= (UP*(float)timer.Delta() * 5.0f);
	}

	if (GetAsyncKeyState('W'))
	{
		Eye += (FORWARD*(float)timer.Delta() * 5.0f);
		At += (FORWARD*(float)timer.Delta() * 5.0f);

	}
	else if (GetAsyncKeyState('S'))
	{
		Eye -= (FORWARD*(float)timer.Delta() * 5.0f);
		At -= (FORWARD*(float)timer.Delta() * 5.0f);
	}

	if (GetAsyncKeyState('D'))
	{
		Eye += (RIGHT*(float)timer.Delta());
		At += (RIGHT*(float)timer.Delta());

	}
	else if (GetAsyncKeyState('A'))
	{
		Eye -= (RIGHT *(float)timer.Delta());
		At -= (RIGHT*(float)timer.Delta());
	}


	//if (GetAsyncKeyState('I'))
	//{
	//	At += (UP*(float)timer.Delta());
	//}
	//else if (GetAsyncKeyState('K'))
	//{ 
	//	At -= (UP*(float)timer.Delta());
	//}

	//if (GetAsyncKeyState('L'))
	//{
	//	At += (RIGHT*(float)timer.Delta() * 4);
	//}
	//else if (GetAsyncKeyState('J'))
	//{
	//	At -= (RIGHT*(float)timer.Delta() * 2);
	//}
	}

//bool LetsDrawSomeStuff::InitDirectInput(HINSTANCE hInstance)
//{
//	 DirectInput8Create(hInstance,
//		DIRECTINPUT_VERSION,
//		IID_IDirectInput8,
//		(void**)&DirectInput,
//		NULL);
//
//	DirectInput->CreateDevice(GUID_SysKeyboard,
//		&DIKeyboard,
//		NULL);
//
//	 DirectInput->CreateDevice(GUID_SysMouse,
//	&DIMouse,
//	NULL);
//
//	 DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
//	 DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
//
//	 DIMouse->SetDataFormat(&c_dfDIMouse);
//	 DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
//
//	return true;
//}

