// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include "Declarations.h"
#include "Mesh.h"
#include "Misc.h"
#include "XTime.h"
#include "SkyBox.h"
// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	ID3D11Device* myDevice = nullptr;
	IDXGISwapChain* mySwapChain = nullptr;
	ID3D11DeviceContext* myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	D3D_DRIVER_TYPE				myDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			myFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11RenderTargetView*		myRenderTargetView = nullptr;

	ID3D11VertexShader*			myVertexShader = nullptr;
	ID3D11VertexShader*			myVertexShaderUV = nullptr;
	ID3D11VertexShader*			SKYMAP_VS;
	ID3D11VertexShader*			myVertexShaderInstance;

	ID3D11PixelShader*			myPixelShader = nullptr;
	ID3D11PixelShader*			mySolidPixelShader = nullptr;
	ID3D11PixelShader*			SKYMAP_PS;

	ID3D11InputLayout*			myVertexLayout = nullptr;

#if CHARIZARD_MESH
	ID3D11Buffer*				charizardVertexBuffer = nullptr;
	ID3D11Buffer*				charizardIndexBuffer = nullptr;
	ID3D11ShaderResourceView*   myTextureRVCharizard = nullptr;

#endif

#if BOX_MESH
	ID3D11Buffer*				boxVertexBuffer = nullptr;
	ID3D11Buffer*				boxIndexBuffer = nullptr;
	ID3D11ShaderResourceView*   myTextureRVBox = nullptr;
#endif

#if SPACESHIP
	ID3D11Buffer*				spaceshipVertexBuffer = nullptr;
	ID3D11Buffer*				spaceshipIndexBuffer = nullptr;
	ID3D11ShaderResourceView*   myTextureRVSpaceShip = nullptr;
#endif

#if PROCEDURAL_SPIRAL
	ID3D11Buffer*				spiralVertexBuffer = nullptr;
#endif

	ID3D11Buffer*				bulbVertexBuffer = nullptr;
	ID3D11Buffer*				bulbIndexBuffer = nullptr;

	ID3D11Buffer*				pyramidVertexBuffer = nullptr;

	ID3D11Buffer*				groundVertexBuffer = nullptr;
	ID3D11Buffer*				groundIndexBuffer = nullptr;

	ID3D11Buffer* skyBoxIndexBuffer;
	ID3D11Buffer* skyBoxVertexBuffer;


	ID3D11ShaderResourceView* myTextureRVSkyBox;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSBackFaceCull;
	ID3D11RasterizerState* RSCullNone;

	XMMATRIX sphereWorld;

	ID3D11Buffer*				myConstantBuffer = nullptr;
	ID3D11Buffer*				myInstanceConstantBuffer = nullptr;
	ID3D11Buffer*				myLightConstantBuffer = nullptr;
	XMMATRIX					worldMatrix;
	XMMATRIX					viewMatrix;
	XMMATRIX					projectionMatrix;

	ID3D11ShaderResourceView*   myTextureRVBulb = nullptr;
	ID3D11ShaderResourceView*   myTextureRVBase = nullptr;
	ID3D11SamplerState*			mySamplerLinear = nullptr;

	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

	float camYaw, camPitch, camRoll;
#if DEBUGGER
	ID3D11Debug *DebugDevice;
#endif


#if WIREFRAME
	ID3D11RasterizerState* WireFrame;
#endif

	InstanceConstantBuffer iCb;

	XTime timer;
	Mesh charizard;
	Mesh box;
	Mesh ground;
	Mesh bulb;
	Mesh spaceShip;
	//SkyBox skyBox;
	Mesh skyBox;
	float fov = 60;
	unsigned int width, height;

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

#pragma region SETTING_RENDER_TARGET_VIEW
			// TODO: Create new DirectX stuff here! (Buffers, Shaders, Layouts, Views, Textures, etc...)
			ID3D11Texture2D* myBackBuffer = nullptr;
			mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBuffer));

			myDevice->CreateRenderTargetView(myBackBuffer, nullptr, &myRenderTargetView);
			myBackBuffer->Release();

			myContext->OMSetRenderTargets(1, &myRenderTargetView, nullptr);
#pragma endregion

#pragma region SETTING_VIEWPORT
			D3D11_VIEWPORT vp;
			attatchPoint->GetClientWidth(width);
			attatchPoint->GetClientHeight(height);
			vp.Width = (FLOAT)width;
			vp.Height = (FLOAT)height;
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0;
			vp.TopLeftY = 0;
			myContext->RSSetViewports(1, &vp);
#pragma endregion

#pragma region CREATING_SHADERS

			HRESULT hr = myDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), nullptr, &myVertexShader);
			hr = myDevice->CreateVertexShader(VS_UVModifier, sizeof(VS_UVModifier), nullptr, &myVertexShaderUV);
			hr = myDevice->CreateVertexShader(VS_SkyBox, sizeof(VS_SkyBox), nullptr, &SKYMAP_VS);
			hr = myDevice->CreateVertexShader(VS_Instance, sizeof(VS_Instance), nullptr, &myVertexShaderInstance);

			myDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &myPixelShader);
			myDevice->CreatePixelShader(SolidPS, sizeof(SolidPS), nullptr, &mySolidPixelShader);
			myDevice->CreatePixelShader(PS_SkyBox, sizeof(PS_SkyBox), nullptr, &SKYMAP_PS);
#pragma endregion

#pragma region CREATE_INPUT_LAYOUT
			// Define the input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{"TANGENT", 0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0 }
			};
			UINT numElements = ARRAYSIZE(layout);

			// Create the input layout
			myDevice->CreateInputLayout(layout, numElements, Trivial_VS, sizeof(Trivial_VS), &myVertexLayout);
			myContext->IASetInputLayout(myVertexLayout);
#pragma endregion

#pragma region DESCRIPTOR_VARIABLES
			D3D11_BUFFER_DESC bd = {};
			D3D11_SUBRESOURCE_DATA InitData = {};
			UINT stride[] = { sizeof(SimpleVertex) };
			UINT offset[] = { 0 };
#pragma endregion

#pragma region WIREFRAME_SETUP
#if WIREFRAME 
			D3D11_RASTERIZER_DESC wfdesc;
			ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
			wfdesc.FillMode = D3D11_FILL_WIREFRAME;
			wfdesc.CullMode = D3D11_CULL_NONE;
			myDevice->CreateRasterizerState(&wfdesc, &WireFrame);
#endif  
#pragma endregion

#pragma region  PYRAMID_INIT
			LoadFromHeader();
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) *hFilePyramid.numVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData = {};
			InitData.pSysMem = hFilePyramid.vertices;
			myDevice->CreateBuffer(&bd, &InitData, &pyramidVertexBuffer);
#pragma endregion

#pragma region SKY_BOX_INIT
			skyBox = Mesh("SkyBox.fbx", 10.0f, myDevice, myTextureRVSkyBox);
			hr = CreateDDSTextureFromFile(myDevice, L"Assets\\OutputCube.dds", nullptr, &myTextureRVSkyBox);
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * skyBox.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData.pSysMem = skyBox.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &skyBoxVertexBuffer);

			// Set vertex buffer

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * skyBox.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = skyBox.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &skyBoxIndexBuffer);
#pragma endregion

#pragma region CHARIZARD_INIT
#if CHARIZARD_MESH
			charizard = Mesh("Charizard.fbx", 5.0f, myDevice, myTextureRVCharizard);
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * charizard.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			//setting subresource data
			InitData.pSysMem = charizard.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &charizardVertexBuffer);

			// Set vertex buffer

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * charizard.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = charizard.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &charizardIndexBuffer);
			// Set index buffer
#endif  
#pragma endregion

#pragma region SPIRAL_INIT
#if PROCEDURAL_SPIRAL
			CreateSpiral();

			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) * ARRAYSIZE(sphereVertices);
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			InitData = {};
			InitData.pSysMem = sphereVertices;
			myDevice->CreateBuffer(&bd, &InitData, &spiralVertexBuffer);

			// Set index buffer
#endif   
#pragma endregion

#pragma region BOX_INIT
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


			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * box.GetNumberOfIndices();
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = box.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &boxIndexBuffer);

#endif  
#pragma endregion

#pragma region SPACESHIP_INIT
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

#endif   
#pragma endregion

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

#pragma region SET_TOPOLOGY
			// Set primitive topology
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);  
#pragma endregion

#pragma region CREATING_CONSTANT_BUFFERS
			// Create the constant buffer
			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			myDevice->CreateBuffer(&bd, nullptr, &myConstantBuffer);

			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(InstanceConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			myDevice->CreateBuffer(&bd, nullptr, &myInstanceConstantBuffer);

			bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(LightConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			myDevice->CreateBuffer(&bd, nullptr, &myLightConstantBuffer);
#pragma endregion

#pragma region SETTING_cBUFFER_VALUES
			for (int i = 0; i < 10; i++)
			{
				iCb.worldArray[i] = XMMatrixTranspose(XMMatrixTranslationFromVector(XMVECTOR{ i*0.3f, i*0.5f, i*0.7f }));
			}
#pragma endregion

#pragma region SETTING_SAMPLER
			D3D11_SAMPLER_DESC sampDesc = {};
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			myDevice->CreateSamplerState(&sampDesc, &mySamplerLinear);
#pragma endregion

#pragma region SETTING_WVP

			// Initialize the world matrix
			worldMatrix = XMMatrixIdentity();

			// Initialize the view matrix
			Eye = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
			At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

			// Initialize the projection matrix
			projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
#pragma endregion

			timer.Restart();
		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	//Release DX Objects aquired from the surface


	// TODO: "Release()" more stuff here!	
	myVertexShader->Release();
	myVertexShaderUV->Release();
	myPixelShader->Release();
	mySolidPixelShader->Release();
	myVertexLayout->Release();

#if CHARIZARD_MESH
	if (charizardVertexBuffer)charizardVertexBuffer->Release();
	if (charizardIndexBuffer)charizardIndexBuffer->Release();
	myTextureRVCharizard->Release();
#endif

#if BOX_MESH
	if (boxVertexBuffer)boxVertexBuffer->Release();
	if (boxIndexBuffer)boxIndexBuffer->Release();
	if (myTextureRVBox)myTextureRVBox->Release();
#endif

#if PROCEDURAL_SPIRAL
	if (spiralVertexBuffer)spiralVertexBuffer->Release();
#endif

	groundVertexBuffer->Release();
	groundIndexBuffer->Release();
	if (myTextureRVBase)myTextureRVBase->Release();
	bulbVertexBuffer->Release();
	bulbIndexBuffer->Release();
	myTextureRVBulb->Release();

	myConstantBuffer->Release();

#if SPACESHIP
	spaceshipVertexBuffer->Release();
	spaceshipIndexBuffer->Release();
	if (myTextureRVSpaceShip)myTextureRVSpaceShip->Release();
#endif // SPACESHIP


	if (mySamplerLinear)mySamplerLinear->Release();

	pyramidVertexBuffer->Release();

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}

#if DEBUGGER
	myDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&DebugDevice));
	DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif

	box.LateDestructor();
	spaceShip.LateDestructor();
	ground.LateDestructor();
	bulb.LateDestructor();

	delete[] hFilePyramid.vertices;

	myRenderTargetView->Release();
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();
}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		timer.Signal();
		// this could be changed during resolution edits, get it every frame
		ID3D11RenderTargetView *myRenderTargetView = nullptr;
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

			CameraMovement();

#if WIREFRAME
			myContext->RSSetState(WireFrame);
#endif

			//myContext->RSSetState(RSBackFaceCull);


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


			//animate cube
			worldMatrix = XMMatrixRotationY(t);

		
			ConstantBuffer cb;
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			cb.mView = XMMatrixTranspose(viewMatrix);
			cb.mProjection = XMMatrixTranspose(projectionMatrix);
			cb.time.x = t;
			cb.vOutputColor = XMFLOAT4(0, 0, 0, 0);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			LightConstantBuffer lCb;
			lCb.lights[0].Position = XMFLOAT4(0, 0, 0, 1);
			lCb.lights[0].Direction = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);

#if DIRECTIONAL_LIGHT_ON
			lCb.lights[0].Color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
#else
			lCb.lights[0].Color =XMFLOAT4(0, 0, 0, 0);
#endif

			lCb.lights[1].Position = XMFLOAT4(0, 0, 0, 1);
			lCb.lights[1].Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
#if DIRECTIONAL_LIGHT_ON
			lCb.lights[1].Color = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
#else
			lCb.lights[1].Color = XMFLOAT4(0, 0, 0, 0);
#endif 

			lCb.lights[2].Position = XMFLOAT4(0, (sin((float)timer.TotalTime()) * 5), 0, 2);
			lCb.lights[2].Range.x = 6.0f;
			lCb.lights[2].Color = XMFLOAT4(0, 0, 1, 1);

			lCb.lights[3].Position = XMFLOAT4(0.0f, 10.0f, 0,2.0f);
			lCb.lights[3].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 10.0f);
			lCb.lights[3].Range.x = 15.0f;
			lCb.lights[3].Range.y = 5.0f;
			lCb.lights[3].Color = XMFLOAT4(0, 1, 0, 1);



#pragma region ROTATE_DIRECTIONAL_LIGHT
			// Rotate the a matrix
			XMMATRIX mRotate = XMMatrixRotationY(-2.0f * t);
			//store the ligh dir in a XMFloat 
			XMVECTOR newLightDir = XMLoadFloat4(&lCb.lights[1].Direction);
			//transform the light dir by the roatation matrix made
			newLightDir = XMVector3Transform(newLightDir, mRotate);
			//copy the values back
			XMStoreFloat4(&lCb.lights[1].Direction, newLightDir);
#pragma endregion

			myContext->UpdateSubresource(myLightConstantBuffer, 0, nullptr, &lCb, 0, 0);

			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetConstantBuffers(1, 1, &myLightConstantBuffer);
			myContext->PSSetSamplers(0, 1, &mySamplerLinear);

			UINT stride[] = { sizeof(SimpleVertex) };
			UINT offset[] = { 0 };

#pragma region SKYBOX_RENDER

			cb.mWorld = XMMatrixTranslationFromVector(Eye);
			myContext->PSSetShaderResources(0, 1, &myTextureRVSkyBox);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
			myContext->VSSetShader(SKYMAP_VS, nullptr, 0);
			myContext->PSSetShader(SKYMAP_PS, nullptr, 0);

			myContext->IASetVertexBuffers(0, 1, &skyBoxVertexBuffer, stride, offset);
			// Set index buffer
			myContext->IASetIndexBuffer(skyBoxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myContext->DrawIndexed(skyBox.GetNumberOfIndices(), 0, 0);


			myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

#pragma endregion

			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->PSSetShader(myPixelShader, nullptr, 0);

#if SPACESHIP
			myContext->PSSetShaderResources(0, 1, &myTextureRVSpaceShip);
#endif
#if CHARIZARD_MESH
			myContext->PSSetShaderResources(0, 1, &myTextureRVCharizard);
#endif
			myContext->PSSetSamplers(0, 1, &mySamplerLinear);


			//Setting buffer description
#if CHARIZARD_MESH

			// Set vertex buffer
			myContext->IASetVertexBuffers(0, 1, &charizardVertexBuffer, stride, offset);

			// Set index buffer
			myContext->IASetIndexBuffer(charizardIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myContext->DrawIndexed(charizard.GetNumberOfIndices(), 0, 0);
#endif


#pragma region BOX_RENDER
#if BOX_MESH
			myContext->VSSetShader(myVertexShaderUV, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBox);

			XMVECTOR boxPosition = { -3, 0.0f,0,0 };
			cb.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(boxPosition));
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(box.GetNumberOfIndices(), 0, 0);
#endif

#pragma endregion

#if DIRECTIONAL_LIGHT_ON
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBox);
			myContext->PSSetShader(mySolidPixelShader, nullptr, 0);

			XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&lCb.lights[1].Direction));
			XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
			mLight = mLightScale * mLight;

			// Update the world variable to reflect the current light
			cb.mWorld = XMMatrixTranspose(mLight);
			cb.vOutputColor = lCb.lights[1].Color;
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(box.GetNumberOfIndices(), 0, 0);
#endif 

#if SPACESHIP
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &myTextureRVSpaceShip);
			myContext->PSSetShader(myPixelShader, nullptr, 0);

			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &spaceshipVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(spaceshipIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(spaceShip.GetNumberOfIndices(), 0, 0);

#endif 
#if PROCEDURAL_SPIRAL
			XMVECTOR spiralPos = { 2,1,-1.5f,0 };
			worldMatrix = XMMatrixTranslationFromVector(spiralPos);
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);

			myContext->IASetVertexBuffers(0, 1, &spiralVertexBuffer, stride, offset);
			myContext->Draw(ARRAYSIZE(sphereVertices), 0);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#endif

#pragma region BULB_RENDER
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBulb);
			myContext->PSSetSamplers(0, 1, &mySamplerLinear);

			worldMatrix = XMMatrixTranslationFromVector(XMVECTOR{ lCb.lights[2].Position.x, lCb.lights[2].Position.y, lCb.lights[2].Position.z, 1});
			cb.mWorld = XMMatrixTranspose(worldMatrix);

			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &bulbVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(bulbIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(bulb.GetNumberOfIndices(), 0, 0);
#pragma endregion

#pragma region GROUND_RENDER			
			//Rendering ground
			XMVECTOR groundPosition = { 0,-1,0,0 };
			worldMatrix = XMMatrixTranslationFromVector(groundPosition);
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBase);

			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &groundVertexBuffer, stride, offset);
			// Set index buffer
			myContext->IASetIndexBuffer(groundIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->DrawIndexed(ground.GetNumberOfIndices(), 0, 0);
#pragma endregion

#pragma region H_FILE_PYRAMID
			XMVECTOR pyramidPosition = { 0,3,0,0 };
			worldMatrix = XMMatrixTranslationFromVector(pyramidPosition);
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			myContext->IASetVertexBuffers(0, 1, &pyramidVertexBuffer, stride, offset);
			//myContext->IASetIndexBuffer(pyramidIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
			myContext->Draw((unsigned int)hFilePyramid.numVertices, 0);
#pragma endregion

#pragma region DRAW_INSTANCED_BOX
			myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
			myContext->VSSetShader(myVertexShaderInstance, nullptr, 0);
			myContext->PSSetShaderResources(0, 1, &myTextureRVBox);
			myContext->VSSetConstantBuffers(1, 1, &myInstanceConstantBuffer);

			myContext->IASetVertexBuffers(0, 1, &boxVertexBuffer, stride, offset);

			myContext->IASetIndexBuffer(boxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			myContext->DrawIndexedInstanced(box.GetNumberOfIndices(), 10, 0, 0, 0);
#pragma endregion

#pragma region LAST_STEP

			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

			myRenderTargetView->Release(); // Free any temp DX handles aquired this frame  
#pragma endregion

		}
	}
}

void LetsDrawSomeStuff::CameraMovement()
{
#pragma region CAMERA_MOVEMENT

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
		Eye += (RIGHT*(float)timer.Delta()* 5.0f);
		At += (RIGHT*(float)timer.Delta()* 5.0f);

	}
	else if (GetAsyncKeyState('A'))
	{
		Eye -= (RIGHT *(float)timer.Delta()* 5.0f);
		At -= (RIGHT*(float)timer.Delta()* 5.0f);
	}
#pragma endregion

#pragma region FOV_ZOOMING
	if (GetAsyncKeyState(VK_UP))
	{
		fov += (10 * timer.Delta());
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		fov -= (10 * timer.Delta());
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
	}
#pragma endregion

#pragma region CAMERA_ROTATION

	if (GetAsyncKeyState('O'))
	{
		camRoll += 1 * timer.Delta();
	}
	else if (GetAsyncKeyState('U'))
	{
		camRoll -= 1 * timer.Delta();
	}

	if (GetAsyncKeyState('I'))
	{
		camPitch += 1 * timer.Delta();
	}
	else if (GetAsyncKeyState('K'))
	{
		camPitch -= 1 * timer.Delta();
	}

	if (GetAsyncKeyState('J'))
	{
		camYaw += 1 * timer.Delta();
	}
	else if (GetAsyncKeyState('L'))
	{
		camYaw -= 1 * timer.Delta();
	}
#pragma endregion

	viewMatrix = XMMatrixLookAtLH(Eye, At, Up);
	viewMatrix*= XMMatrixRotationRollPitchYaw(camPitch, camYaw, camRoll);
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

