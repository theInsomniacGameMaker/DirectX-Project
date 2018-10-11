// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include "Declarations.h"
#include "Mesh.h"
#include "Misc.h"
#include "XTime.h"
#include "TextureRenderer.h"
#include "D3DObject.h"
#include "ScreenQuad.h"
// Simple Container class to make life easier/cleaner
class LetsDrawSomeStuff
{
	// variables here
	GW::GRAPHICS::GDirectX11Surface* mySurface = nullptr;
	// Gettting these handles from GDirectX11Surface will increase their internal refrence counts, be sure to "Release()" them when done!
	CComPtr<ID3D11Device> myDevice = nullptr;
	CComPtr<IDXGISwapChain> mySwapChain = nullptr;
	CComPtr<ID3D11DeviceContext> myContext = nullptr;

	// TODO: Add your own D3D11 variables here (be sure to "Release()" them when done!)
	D3D_DRIVER_TYPE				myDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL			myFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	CComPtr < ID3D11RenderTargetView>		myRenderTargetView = nullptr;

	CComPtr<ID3D11VertexShader>		myVertexShader = nullptr;
	CComPtr<ID3D11VertexShader>		myVertexShaderUV = nullptr;
	CComPtr<ID3D11VertexShader>		SKYMAP_VS = nullptr;
	CComPtr<ID3D11VertexShader>		myVertexShaderInstance = nullptr;
	CComPtr<ID3D11VertexShader>		myVertexShaderWave = nullptr;
	CComPtr<ID3D11VertexShader>		myVertexShaderPassThrough = nullptr;
	CComPtr<ID3D11VertexShader>		myVertexShaderReflective = nullptr;
	CComPtr<ID3D11VertexShader>		myVertexShaderScreenSpace = nullptr;

	CComPtr<ID3D11PixelShader>		myPixelShader = nullptr;
	CComPtr<ID3D11PixelShader>		mySolidPixelShader = nullptr;
	CComPtr<ID3D11PixelShader>		SKYMAP_PS = nullptr;
	CComPtr<ID3D11PixelShader>		myPixelShaderMultitexturing = nullptr;
	CComPtr<ID3D11PixelShader>		myPixelShaderNoLighting = nullptr;
	CComPtr<ID3D11PixelShader>		myPixelShaderReflective = nullptr;
	CComPtr<ID3D11PixelShader>		myPixelShaderPostProcessing = nullptr;

	CComPtr<ID3D11GeometryShader>		myGeometryShader = nullptr;
	CComPtr<ID3D11GeometryShader>		nullGeometryShader = nullptr;

	CComPtr < ID3D11InputLayout>			myVertexLayout = nullptr;
	CComPtr < ID3D11ShaderResourceView>	myTextureRVPMT[2];

	CComPtr<ID3D11Buffer>				myConstantBuffer = nullptr;
	CComPtr<ID3D11Buffer>				myInstanceConstantBuffer = nullptr;
	CComPtr<ID3D11Buffer>				myLightConstantBuffer = nullptr;
	XMMATRIX					worldMatrix;
	XMMATRIX					viewMatrix;
	XMMATRIX					projectionMatrix;

	CComPtr < ID3D11SamplerState>			mySamplerLinear = nullptr;

	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

	ScreenQuad* screenQuad;
	TextureRenderer *textureRenderer;
	TextureRenderer *mainTextureRenderer;
	D3DObject *feraligtr;
	D3DObject *skyBox;
	D3DObject *ground;
	D3DObject* box;
	D3DObject* bulb;
	D3DObject* spaceShip;
	D3DObject* quad;
	D3DObject* quad1;
	D3DObject* quad2;
	D3DObject* reflectiveCube;
	D3DObject* secondaryScreen;

	InstanceConstantBuffer iCb;
	LightConstantBuffer lCb;
	ConstantBuffer cb;

	//SimpleVertex screenQuad[4];

	XTime xTimer;

	float camYaw, camPitch, camRoll;
	float moveX, moveY, moveZ;
	float fov = 60;
	unsigned int width, height;

#if DEBUGGER
	ID3D11Debug *DebugDevice;
#endif


public:
	// Init
	LetsDrawSomeStuff(GW::SYSTEM::GWindow* attatchPoint);
	// Shutdown
	~LetsDrawSomeStuff();
	// Draw
	void Render();
	//Camera Movement
	void CameraMovement();
	//Create the constant buffers
	void CreateConstantBuffers();
	//Update the Light Buffers
	void UpdateLightBuffer();
	//Update the constant buffer
	void UpdateConstantBuffer();
	//Create the view port
	void CreateViewPort(GW::SYSTEM::GWindow* attatchPoint);
	//Set the render target view (redundant)
	void SetupRenderTargetView();
	//Create all shaders
	void CreateShaders();
	//Create Input Layout
	void CreateInputLayout();
	//Create Sampler
	void CreateSampler();
	//Setting up World, View and Projection Matrices
	void SetupWVP();
	//Update the camera
	void UpdateCamera();

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
			mySurface->GetDevice((void**)&myDevice.p);
			mySurface->GetSwapchain((void**)&mySwapChain.p);
			mySurface->GetContext((void**)&myContext.p);

			SetupRenderTargetView();

			CreateViewPort(attatchPoint);

			CreateShaders();

			CreateDDSTextureFromFile(myDevice, L"Assets\\PirateBox.dds", nullptr, &myTextureRVPMT[0]);
			CreateDDSTextureFromFile(myDevice, L"Assets\\Brick.dds", nullptr, &myTextureRVPMT[1]);

			CreateInputLayout();

			feraligtr = new D3DObject("Feraligatr.fbx", 5.0f, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader, myConstantBuffer);

			skyBox = new D3DObject("SkyBox.fbx", 10.0f, myDevice, myContext, SKYMAP_VS, SKYMAP_PS, nullGeometryShader, myConstantBuffer);
			skyBox->UpdateTexture("OutputCube");

			ground = new D3DObject("Ground.fbx", 10.0f, myDevice, myContext, myVertexShaderWave, myPixelShader, nullGeometryShader, myConstantBuffer);

			box = new D3DObject("cube.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderMultitexturing, nullGeometryShader, myConstantBuffer);

			bulb = new D3DObject("Bulb.fbx", 1.0f / 5, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader, myConstantBuffer);

			spaceShip = new D3DObject("Galaga Fighter.fbx", 1.0f / 4, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader, myConstantBuffer);

			quad = new D3DObject("Q.fbx", 1.0f, myDevice, myContext, myVertexShader, myPixelShaderNoLighting, nullGeometryShader, myConstantBuffer);

			quad1 = new D3DObject("Q.fbx", 5.0f, myDevice, myContext, myVertexShaderPassThrough, myPixelShaderNoLighting, myGeometryShader, myConstantBuffer);

			quad2 = new D3DObject("cube.fbx", 1/50.0f, myDevice, myContext, myVertexShader, myPixelShaderNoLighting, nullGeometryShader, myConstantBuffer);

			reflectiveCube = new D3DObject("utah-teapot.fbx", 0.1f, myDevice, myContext, myVertexShaderReflective, myPixelShaderReflective, nullGeometryShader, myConstantBuffer);
			reflectiveCube->UpdateTexture("OutputCube");

			secondaryScreen = new D3DObject("cube.fbx", 1/25.0f, myDevice, myContext, myVertexShader, myPixelShaderPostProcessing, nullGeometryShader, myConstantBuffer);

			screenQuad = new ScreenQuad(myDevice, myContext, myVertexShaderScreenSpace, myPixelShaderPostProcessing, nullGeometryShader);

			textureRenderer = new TextureRenderer(myDevice, width, height);
			mainTextureRenderer = new TextureRenderer(myDevice, width, height);
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			CreateConstantBuffers();

			CreateSampler();

			SetupWVP();

			xTimer.Restart();
		}
	}
}

// Shutdown
LetsDrawSomeStuff::~LetsDrawSomeStuff()
{
	//Release DX Objects aquired from the surface

	/*myVertexShader->Release();
	myVertexShaderUV->Release();
	myPixelShader->Release();
	mySolidPixelShader->Release();
	myVertexLayout->Release();
	myVertexShaderReflective->Release();
	SKYMAP_VS->Release();
	myVertexShaderInstance->Release();
	myVertexShaderWave->Release();
	myVertexShaderPassThrough->Release();

	SKYMAP_PS->Release();
	myPixelShaderMultitexturing->Release();
	myPixelShaderNoLighting->Release();
	myPixelShaderReflective->Release();
	myGeometryShader->Release();

	myTextureRVPMT[0]->Release();
	myTextureRVPMT[1]->Release();

	myLightConstantBuffer->Release();
	myInstanceConstantBuffer->Release();
	myConstantBuffer->Release();*/

	//if (mySamplerLinear)mySamplerLinear->Release();

	if (mySurface) // Free Gateware Interface
	{
		mySurface->DecrementCount(); // reduce internal count (will auto delete on Zero)
		mySurface = nullptr; // the safest way to fly
	}

#if DEBUGGER
	myDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&DebugDevice));
	DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	DebugDevice->Release();
#endif

	delete feraligtr;
	delete skyBox;
	delete box;
	delete bulb;
	delete ground;
	delete spaceShip;
	delete quad;
	delete quad1;
	delete quad2;
	delete reflectiveCube;
	delete secondaryScreen;
	delete textureRenderer;

	/*myRenderTargetView->Release();
	myDevice->Release();
	mySwapChain->Release();
	myContext->Release();*/

}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		xTimer.Signal();
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

			UpdateConstantBuffer();
			UpdateLightBuffer();


			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer.p);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer.p);
			myContext->PSSetConstantBuffers(1, 1, &myLightConstantBuffer.p);
			myContext->PSSetSamplers(0, 1, &mySamplerLinear.p);
			myContext->GSSetShader(nullGeometryShader, nullptr, 0);
			myContext->GSSetShader(myGeometryShader, nullptr, 0);
			myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer.p);
			
			mainTextureRenderer->Clear(myContext, myDepthStencilView, XMFLOAT4(0, 0, 0, 0));
			mainTextureRenderer->BeginRender(myContext);

			skyBox->SetPosition(Eye, cb, myConstantBuffer);
			skyBox->RenderIndexed();
			//myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.
			mainTextureRenderer->ClearDPV(myContext);

			feraligtr->SetLocalRotation(XMVECTOR{ 5,0,2,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime());
			feraligtr->RenderIndexed();

			ground->SetPosition(XMVECTOR{ 0,-0.5,0,0 }, cb, myConstantBuffer);
			ground->RenderIndexed();

			box->SetPosition(XMVECTOR{ -5,0,0,1 }, cb, myConstantBuffer);
			box->UpdateVS(myVertexShader);
			box->UpdatePS(myPixelShaderMultitexturing);
			box->RenderIndexedMulitexture(myTextureRVPMT);

			reflectiveCube->SetLocalRotation(XMVECTOR{ 0,5.0f,0.0f,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime()/2.0f,(float)xTimer.TotalTime()/2.0f);
			reflectiveCube->RenderIndexed();

			
#if DIRECTIONAL_LIGHT_ON
			box->UpdatePS(mySolidPixelShader);
			box->UpdateVS(myVertexShader);
			XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&lCb.lights[1].Direction));
			XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
			mLight = mLightScale * mLight;
			box->SetPosition(mLight, cb, myConstantBuffer);
			cb.vOutputColor = lCb.lights[1].Color;
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
			box->RenderIndexed();
#endif 
			quad1->SetPosition(XMVECTOR{ 3, 0, 0, 0 }, cb, myConstantBuffer);
			quad1->RenderIndexed(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

			bulb->SetPosition(XMVECTOR{ lCb.lights[2].Position.x, lCb.lights[2].Position.y, lCb.lights[2].Position.z, 1 }, cb, myConstantBuffer);
			bulb->RenderIndexed();

			myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
			box->UpdateVS(myVertexShaderInstance);
			box->UpdatePS(myPixelShaderMultitexturing);
			box->RenderInstanced(10, myInstanceConstantBuffer);


			textureRenderer->MoveCamera(cb, myConstantBuffer, myContext);
			textureRenderer->Clear(myContext, nullptr, XMFLOAT4(1, 1, 1, 1));
			textureRenderer->BeginRender(myContext);
#if SPACESHIP
			spaceShip->SetLocalRotation(XMVECTOR{ 100,0,0,0 }, cb, myConstantBuffer,(float)xTimer.TotalTime(), (float)xTimer.TotalTime());
			spaceShip->RenderIndexed();
#endif 
			textureRenderer->EndRender(myContext, myRenderTargetView, myDepthStencilView);

			UpdateCamera();

			quad2->UpdateTexture(textureRenderer->pCTexture);
			quad2->SetPosition(XMVECTOR{ 5, 2, -3, 1 }, cb, myConstantBuffer);
			//quad2->RenderIndexedWithDynamicSRV(textureRenderer->pCTexture);

			mainTextureRenderer->EndRender(myContext, myRenderTargetView, myDepthStencilView);

			secondaryScreen->SetPosition(XMVECTOR{ 0, 0, 3 }, cb, myConstantBuffer);
			secondaryScreen->UpdateTexture(mainTextureRenderer->pCTexture);
			//secondaryScreen->RotateAndMove(XMMatrixRotationX(60), XMVECTOR{ 0, 3, -1 }, cb, myConstantBuffer);
			//secondaryScreen->RenderIndexed();
			screenQuad->UpdateTexture(mainTextureRenderer->pCTexture);
			screenQuad->Render();


			mainTextureRenderer->pResView = { nullptr };
			myContext->PSSetShaderResources(0, 1, &mainTextureRenderer->pResView.p);
			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display
			myRenderTargetView->Release(); // Free any temp DX handles aquired this frame  

		}
	}
}

void LetsDrawSomeStuff::CameraMovement()
{
#pragma region CAMERA_MOVEMENT

	if (GetAsyncKeyState('E'))
	{
		moveY += (float)xTimer.Delta()*5.0f;
	}
	else if (GetAsyncKeyState('Q'))
	{
		moveY -= (float)xTimer.Delta()*5.0f;
	}

	if (GetAsyncKeyState('W'))
	{
		moveZ += (float)xTimer.Delta()*5.0f;
	}
	else if (GetAsyncKeyState('S'))
	{
		moveZ -= (float)xTimer.Delta()*5.0f;
	}

	if (GetAsyncKeyState('D'))
	{
		moveX += (float)xTimer.Delta()*5.0f;
	}
	else if (GetAsyncKeyState('A'))
	{
		moveX -= (float)xTimer.Delta()*5.0f;
	}

#pragma endregion

#pragma region FOV_ZOOMING
	if (GetAsyncKeyState(VK_UP))
	{
		fov += (10.0f * (float)xTimer.Delta());
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		fov -= (10 * (float)xTimer.Delta());
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
	}
#pragma endregion

#pragma region CAMERA_ROTATION

	if (GetAsyncKeyState('O'))
	{
		camRoll += 1 * (float)xTimer.Delta();
	}
	else if (GetAsyncKeyState('U'))
	{
		camRoll -= 1 * (float)xTimer.Delta();
	}

	if (GetAsyncKeyState('I'))
	{
		camPitch += 1 * (float)xTimer.Delta();
	}
	else if (GetAsyncKeyState('K'))
	{
		camPitch -= 1 * (float)xTimer.Delta();
	}

	if (GetAsyncKeyState('J'))
	{
		camYaw += 1 * (float)xTimer.Delta();
	}
	else if (GetAsyncKeyState('L'))
	{
		camYaw -= 1 * (float)xTimer.Delta();
	}
#pragma endregion


	XMMATRIX camRotationMatrix = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(camPitch, camYaw, camRoll));
	At = XMVector3TransformCoord(FORWARD, camRotationMatrix);
	At = XMVector3Normalize(At);


	XMVECTOR camRight = XMVector3TransformCoord(RIGHT, camRotationMatrix);
	XMVECTOR camForward = XMVector3TransformCoord(FORWARD, camRotationMatrix);
	XMVECTOR camUp = XMVector3Cross(camForward, camRight);

	Eye += moveX * camRight;
	Eye += moveZ * camForward;
	Eye += moveY * camUp;
	Up = camUp;
	moveX = 0.0f;
	moveZ = 0.0f;
	moveY = 0;

	At = Eye + At;

	viewMatrix = XMMatrixLookAtLH(Eye, At, Up);
}

void LetsDrawSomeStuff::CreateConstantBuffers()
{
	D3D11_BUFFER_DESC bd = {};

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

	int j = -5;
	for (int i = 0; i < 10; i++)
	{
		iCb.worldArray[i] = XMMatrixTranspose(XMMatrixTranslationFromVector(XMVECTOR{ j*0.3f, j*1.5f, j*0.7f }));
		j++;
	}
}

void LetsDrawSomeStuff::UpdateLightBuffer()
{
	lCb.lights[0].Position = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[0].Direction = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);

#if DIRECTIONAL_LIGHT_ON
	lCb.lights[0].Color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
#else
	lCb.lights[0].Color = XMFLOAT4(0, 0, 0, 0);
#endif

	lCb.lights[1].Position = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[1].Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
#if DIRECTIONAL_LIGHT_ON
	lCb.lights[1].Color = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
#else
	lCb.lights[1].Color = XMFLOAT4(0, 0, 0, 0);
#endif 

	lCb.lights[2].Position = XMFLOAT4(0, (sin((float)xTimer.TotalTime()) * 5), 0, 2);
	lCb.lights[2].Range.x = 6.0f;
	lCb.lights[2].Color = XMFLOAT4(0, 0, 1, 1);

	lCb.lights[3].Position = XMFLOAT4(sin((float)xTimer.TotalTime() * 2), 2.0f, cos((float)xTimer.TotalTime() * 2), 3.0f);
	lCb.lights[3].Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 10.0f);
	lCb.lights[3].Range.x = 0.9f;
	lCb.lights[3].Range.y = 0.8f;
	lCb.lights[3].Range.z = 10.0f;
	lCb.lights[3].Color = XMFLOAT4(0, 1, 0, 1);

	lCb.lights[4].Position = XMFLOAT4(3.0f, 2.0f, 0, 3.0f);
	lCb.lights[4].Direction = XMFLOAT4(sin((float)xTimer.TotalTime()), -1.0f, 0.0f, 10.0f);
	lCb.lights[4].Range.x = 0.9f;
	lCb.lights[4].Range.y = 0.8f;
	lCb.lights[4].Range.z = 10.0f;
	lCb.lights[4].Color = XMFLOAT4(1, 1, 1, 1);

	// Rotate the a matrix
	XMMATRIX mRotate = XMMatrixRotationY(-2.0f * (float)xTimer.TotalTime());
	//store the ligh dir in a XMFloat 
	XMVECTOR newLightDir = XMLoadFloat4(&lCb.lights[1].Direction);
	//transform the light dir by the roatation matrix made
	newLightDir = XMVector3Transform(newLightDir, mRotate);
	//copy the values back
	XMStoreFloat4(&lCb.lights[1].Direction, newLightDir);

	myContext->UpdateSubresource(myLightConstantBuffer, 0, nullptr, &lCb, 0, 0);

}

void LetsDrawSomeStuff::UpdateConstantBuffer()
{

	cb.mWorld = XMMatrixTranspose(worldMatrix);
	cb.mView = XMMatrixTranspose(viewMatrix);
	cb.mProjection = XMMatrixTranspose(projectionMatrix);
	cb.time.x = (float)xTimer.TotalTime();
	cb.vOutputColor = XMFLOAT4(0, 0, 0, 0);
	myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
}

void LetsDrawSomeStuff::CreateViewPort(GW::SYSTEM::GWindow* attatchPoint)
{
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
}

void LetsDrawSomeStuff::SetupRenderTargetView()
{
	ID3D11Texture2D* myBackBuffer = nullptr;
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&myBackBuffer));

	myDevice->CreateRenderTargetView(myBackBuffer, nullptr, &myRenderTargetView);
	myBackBuffer->Release();

	myContext->OMSetRenderTargets(1, &myRenderTargetView.p, nullptr);
}

void LetsDrawSomeStuff::CreateShaders()
{
	HRESULT hr;
	hr = myDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), nullptr, &myVertexShader);
	hr = myDevice->CreateVertexShader(VS_UVModifier, sizeof(VS_UVModifier), nullptr, &myVertexShaderUV);
	hr = myDevice->CreateVertexShader(VS_SkyBox, sizeof(VS_SkyBox), nullptr, &SKYMAP_VS);
	hr = myDevice->CreateVertexShader(VS_Instance, sizeof(VS_Instance), nullptr, &myVertexShaderInstance);
	hr = myDevice->CreateVertexShader(VS_PositionModifier, sizeof(VS_PositionModifier), nullptr, &myVertexShaderWave);
	hr = myDevice->CreateVertexShader(VS_PassThrough, sizeof(VS_PassThrough), nullptr, &myVertexShaderPassThrough);
	hr = myDevice->CreateVertexShader(VS_Reflective, sizeof(VS_Reflective), nullptr, &myVertexShaderReflective);
	hr = myDevice->CreateVertexShader(VS_ScreenSpace, sizeof(VS_ScreenSpace), nullptr, &myVertexShaderScreenSpace);

	hr = myDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &myPixelShader);
	hr = myDevice->CreatePixelShader(SolidPS, sizeof(SolidPS), nullptr, &mySolidPixelShader);
	hr = myDevice->CreatePixelShader(PS_SkyBox, sizeof(PS_SkyBox), nullptr, &SKYMAP_PS);
	hr = myDevice->CreatePixelShader(PS_Multitexturing, sizeof(PS_Multitexturing), nullptr, &myPixelShaderMultitexturing);
	hr = myDevice->CreatePixelShader(PS_NoLighting, sizeof(PS_NoLighting), nullptr, &myPixelShaderNoLighting);
	hr = myDevice->CreatePixelShader(PS_Reflective, sizeof(PS_Reflective), nullptr, &myPixelShaderReflective);
	hr = myDevice->CreatePixelShader(PS_PostProcessing, sizeof(PS_PostProcessing), nullptr, &myPixelShaderPostProcessing);

	hr = myDevice->CreateGeometryShader(GS_PointToQuad, sizeof(GS_PointToQuad), nullptr, &myGeometryShader);
}

void LetsDrawSomeStuff::CreateInputLayout()
{
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
}

void LetsDrawSomeStuff::CreateSampler()
{
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	myDevice->CreateSamplerState(&sampDesc, &mySamplerLinear);
}

void LetsDrawSomeStuff::SetupWVP()
{
	// Initialize the world matrix
	worldMatrix = XMMatrixIdentity();

	// Initialize the view matrix
	Eye = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
	At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

	// Initialize the projection matrix
	projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
}

void LetsDrawSomeStuff::UpdateCamera()
{
	cb.mView = XMMatrixTranspose(viewMatrix);
	myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
}
