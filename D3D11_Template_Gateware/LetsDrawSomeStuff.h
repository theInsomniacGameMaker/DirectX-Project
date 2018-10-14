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
	CComPtr<ID3D11Device>					myDevice = nullptr;
	CComPtr<IDXGISwapChain>					mySwapChain = nullptr;
	CComPtr<ID3D11DeviceContext>			myContext = nullptr;

	D3D_DRIVER_TYPE							myDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL						myFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	CComPtr < ID3D11RenderTargetView>		myRenderTargetView = nullptr;
	
	//All vertex shaders
	CComPtr<ID3D11VertexShader>				myVertexShader = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderUV = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderSkyBox = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderInstance = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderWave = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderPassThrough = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderReflective = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderScreenSpace = nullptr;

	//All Pixel Shaders
	CComPtr<ID3D11PixelShader>				myPixelShader = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderSolid = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderSkyBox = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderMultitexturing = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderNoLighting = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderReflective = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderPostProcessing = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderSpecular = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderTransparent = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderEmissive = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderTransparentRejector = nullptr;

	//All Geometry Shaders
	CComPtr<ID3D11GeometryShader>			myGeometryShaderPoint = nullptr;
	CComPtr<ID3D11GeometryShader>			myGeometryShaderTriangle = nullptr;
	CComPtr<ID3D11GeometryShader>			nullGeometryShader = nullptr; //Used for objects that do not use a GS

	//Vertex Layout
	CComPtr < ID3D11InputLayout>			myVertexLayout = nullptr;
	//SRV (texture) for multitexturing
	CComPtr < ID3D11ShaderResourceView>		myTextureRVPMT[2];

	CComPtr < ID3D11ShaderResourceView>		myTextureEmissive;

	//Per Object Buffer
	CComPtr<ID3D11Buffer>					myConstantBuffer = nullptr;
	//Per Instance Buffer
	CComPtr<ID3D11Buffer>					myInstanceConstantBuffer = nullptr;
	//Light Buffer
	CComPtr<ID3D11Buffer>					myLightConstantBuffer = nullptr;
	//Blend State Object
	CComPtr<ID3D11BlendState>				transparencyBlendState = nullptr;

	//Sampler State
	CComPtr < ID3D11SamplerState>			mySamplerLinear = nullptr;

	//Camera matrices
	XMMATRIX								worldMatrix;
	XMMATRIX								viewMatrix;
	XMMATRIX								projectionMatrix;

	//View Matrix Vectors
	XMVECTOR Eye;
	XMVECTOR At;
	XMVECTOR Up;

	//Quad that is on a screen to appl Post Processing
	ScreenQuad*		screenQuad;
	ScreenQuad*		screenQuadRightTop;

	//RTT variables
	TextureRenderer*	textureRenderer;
	TextureRenderer*	mainTextureRenderer;
	TextureRenderer*	rightTopRTT;

	//All 3D Objects 
	D3DObject*		feraligtr;
	D3DObject*		skyBox;
	D3DObject*		ground;
	D3DObject*		box;
	D3DObject*		bulb;
	D3DObject*		spaceShip;
	D3DObject*		cubeGS;
	D3DObject*		spaceShipRTT;
	D3DObject*		reflectiveTeapot;
	D3DObject*		emissiveTeapot;
	D3DObject*		secondaryScreen;
	D3DObject*		plant;

	D3DObject*		choppedWood;
	D3DObject*		closedSack;

	//Constant Buffers
	InstanceConstantBuffer	iCb;
	LightConstantBuffer		lCb;
	ConstantBuffer			cb;



	//The vector that contains all the transparent objects
	vector<D3DObject*>		transparentObjects;
	//Timer class
	XTime					xTimer;

	float camYaw, camPitch, camRoll;
	float moveX, moveY, moveZ;
	float fov = 60;
	float nearPlane = 0.01, farPlane = 100;
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
	//Create Blend State
	void CreateBlendState();
	//Sort the object according to the position from the camera
	void RenderTransparentObjects();

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

			//SetupRenderTargetView();

			CreateViewPort(attatchPoint);

			CreateShaders();

			CreateDDSTextureFromFile(myDevice, L"Assets\\PirateBox.dds", nullptr, &myTextureRVPMT[0]);
			CreateDDSTextureFromFile(myDevice, L"Assets\\Brick.dds", nullptr, &myTextureRVPMT[1]);

			HRESULT hr = CreateDDSTextureFromFile(myDevice, L"Assets\\Lave_EMISSIVEMAP.dds", nullptr, &myTextureEmissive);

			CreateInputLayout();

			transparentObjects.resize(3);

			feraligtr = new D3DObject("Charizard.fbx", 5.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular, nullGeometryShader, myConstantBuffer);

			skyBox = new D3DObject("SkyBox.fbx", 10.0f, myDevice, myContext, myVertexShaderSkyBox, myPixelShaderSkyBox, nullGeometryShader, myConstantBuffer);
			skyBox->UpdateTexture("OutputCube");

			ground = new D3DObject("Ground.fbx", 10.0f, myDevice, myContext, myVertexShaderWave, myPixelShader, nullGeometryShader, myConstantBuffer);

			box = new D3DObject("cube.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderMultitexturing, nullGeometryShader, myConstantBuffer);

			bulb = new D3DObject("Bulb.fbx", 1.0f / 5, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader, myConstantBuffer);

			spaceShip = new D3DObject("Galaga Fighter.fbx", 1.0f / 4, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader, myConstantBuffer);

			cubeGS = new D3DObject("Q.fbx", 5.0f, myDevice, myContext, myVertexShaderPassThrough, myPixelShaderNoLighting, myGeometryShaderPoint, myConstantBuffer);

			spaceShipRTT = new D3DObject("cube.fbx", 1/50.0f, myDevice, myContext, myVertexShader, myPixelShaderNoLighting, nullGeometryShader, myConstantBuffer);

			reflectiveTeapot = new D3DObject("utah-teapot.fbx", 0.1f, myDevice, myContext, myVertexShaderReflective, myPixelShaderReflective, nullGeometryShader, myConstantBuffer);
			reflectiveTeapot->UpdateTexture("OutputCube");

			emissiveTeapot = new D3DObject("cube.fbx", 1/50.0f, myDevice, myContext, myVertexShader, 
				myPixelShaderEmissive, nullGeometryShader, myConstantBuffer);
			emissiveTeapot->UpdateTexture("Lava");

			choppedWood = new D3DObject("Chopped_Wood_Pile.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader,
				myPixelShader, nullGeometryShader, myConstantBuffer, "Chopped_Wood_Pile_Albedo");

			closedSack = new D3DObject("Closed_Sack.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader,
				myPixelShader, nullGeometryShader, myConstantBuffer, "Closed_Sack_Albedo");


			plant = new D3DObject("Parviflora.fbx", 1.0f/15.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparentRejector, nullGeometryShader, myConstantBuffer);
			plant->UpdateTexture("Parviflora_diffuse");

			transparentObjects[0] = new D3DObject("cube.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparent, nullGeometryShader, myConstantBuffer);
			transparentObjects[1] = new D3DObject("cube.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparent, nullGeometryShader, myConstantBuffer);
			transparentObjects[2] = new D3DObject("cube.fbx", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparent, nullGeometryShader, myConstantBuffer);

			transparentObjects[0]->UpdateTexture("Energy");
			transparentObjects[1]->UpdateTexture("Energy1");
			transparentObjects[2]->UpdateTexture("Energy");
			

			XMFLOAT3 positions[4];
			positions[0] = XMFLOAT3(-1, 1,0);
			positions[1] = XMFLOAT3(0, 1, 0);
			positions[2] = XMFLOAT3(-1, 0, 0);
			positions[3] = XMFLOAT3(0, 0, 0);

			screenQuad = new ScreenQuad(myDevice, myContext, myVertexShaderScreenSpace, myPixelShaderNoLighting, nullGeometryShader, positions);

			textureRenderer = new TextureRenderer(myDevice, width, height);
			mainTextureRenderer = new TextureRenderer(myDevice, width, height);
			rightTopRTT = new TextureRenderer(myDevice, width, height);
			
			positions[0] = XMFLOAT3(0, 1, 0);
			positions[1] = XMFLOAT3(1, 1, 0);
			positions[2] = XMFLOAT3(0, 0, 0);
			positions[3] = XMFLOAT3(1, 0, 0);
			screenQuadRightTop = new ScreenQuad(myDevice, myContext, myVertexShaderScreenSpace, myPixelShaderNoLighting, nullGeometryShader, positions);

			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			CreateBlendState();

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

	myVertexShader.Release();
	myVertexShaderUV.Release();
	myPixelShader.Release();
	myPixelShaderSolid.Release();
	myVertexLayout.Release();
	myVertexShaderReflective.Release();
	myVertexShaderSkyBox.Release();
	myVertexShaderInstance.Release();
	myVertexShaderWave.Release();
	myVertexShaderPassThrough.Release();

	myPixelShaderSkyBox.Release();
	myPixelShaderMultitexturing.Release();
	myPixelShaderNoLighting.Release();
	myPixelShaderReflective.Release();
	myGeometryShaderPoint.Release();

	myTextureRVPMT[0].Release();
	myTextureRVPMT[1].Release();

	myLightConstantBuffer.Release();
	myInstanceConstantBuffer.Release();
	myConstantBuffer.Release();

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
	delete cubeGS;
	delete spaceShipRTT;
	delete plant;
	delete closedSack;
	delete choppedWood;
	delete reflectiveTeapot;
	delete emissiveTeapot;

	delete textureRenderer;
	delete mainTextureRenderer;
	delete screenQuad;
	delete screenQuadRightTop;
	delete rightTopRTT;

	for (int i = 0; i < transparentObjects.size(); i++)
	{
		delete transparentObjects[i];
	}

	//delete transparentCube1;
	//delete transparentCube2;
	//delete transparentCube3;
	myRenderTargetView.Release();
	myDevice.Release();
	mySwapChain.Release();
	myContext.Release();

}

// Draw
void LetsDrawSomeStuff::Render()
{
	if (mySurface) // valid?
	{
		xTimer.Signal();
		// this could be changed during resolution edits, get it every frame
		CComPtr<ID3D11RenderTargetView> myRenderTargetView = nullptr;
		CComPtr<ID3D11DepthStencilView> myDepthStencilView = nullptr;
		if (G_SUCCESS(mySurface->GetRenderTarget((void**)&myRenderTargetView)))
		{
			// Grab the Z Buffer if one was requested
			if (G_SUCCESS(mySurface->GetDepthStencilView((void**)&myDepthStencilView)))
			{
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

				myDepthStencilView.Release();
			}

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView.p };
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
			myContext->GSSetShader(myGeometryShaderPoint, nullptr, 0);
			myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer.p);
			
			mainTextureRenderer->Clear(myContext, myDepthStencilView, XMFLOAT4(0, 0, 0, 0));
			mainTextureRenderer->BeginRender(myContext, myRenderTargetView);


			//changing blending factor
			float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };

			//Set the default blend state (no blending) for opaque objects
			myContext->OMSetBlendState(0, 0, 0xffffffff);

			//Render opaque objects//

			skyBox->SetPosition(Eye, cb, myConstantBuffer);
			skyBox->RenderIndexed();
			mainTextureRenderer->ClearDPV(myContext);

			feraligtr->SetLocalRotation(XMVECTOR{ 5,0,2,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime());
			feraligtr->RenderIndexed();

			ground->SetPosition(XMVECTOR{ 0,-0.5,0,0 }, cb, myConstantBuffer);
			ground->RenderIndexed();

			box->SetPosition(XMVECTOR{ -5,0,0,1 }, cb, myConstantBuffer);
			box->UpdateVS(myVertexShader);
			box->UpdatePS(myPixelShaderMultitexturing);
			box->RenderIndexedMulitexture(myTextureRVPMT);

			reflectiveTeapot->SetLocalRotation(XMVECTOR{ 0,5.0f,0.0f,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime()/2.0f,(float)xTimer.TotalTime()/2.0f);
			reflectiveTeapot->RenderIndexed();

			box->SetPosition(XMVECTOR{ -5,2,0,0 }, cb, myConstantBuffer);
			box->UpdateVS(myVertexShaderPassThrough);
			box->UpdateGS(myGeometryShaderTriangle);
			box->RenderIndexedWithGS(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			box->UpdateGS(nullGeometryShader);

#if DIRECTIONAL_LIGHT_ON
			box->UpdatePS(myPixelShaderSolid);
			box->UpdateVS(myVertexShader);
			XMMATRIX mLight = XMMatrixTranslationFromVector(5.0f * XMLoadFloat4(&lCb.lights[1].Direction));
			XMMATRIX mLightScale = XMMatrixScaling(0.2f, 0.2f, 0.2f);
			mLight = mLightScale * mLight;
			box->SetPosition(mLight, cb, myConstantBuffer);
			cb.vOutputColor = lCb.lights[1].Color;
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
			box->RenderIndexed();
#endif 
			cubeGS->SetPosition(XMVECTOR{ 3, 0, 0, 0 }, cb, myConstantBuffer);
			cubeGS->RenderIndexedWithGS(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

			bulb->SetPosition(XMVECTOR{ lCb.lights[2].Position.x, lCb.lights[2].Position.y, lCb.lights[2].Position.z, 1 }, cb, myConstantBuffer);
			bulb->RenderIndexed();

			myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
			box->UpdateVS(myVertexShaderInstance);
			box->UpdatePS(myPixelShaderMultitexturing);
			box->RenderInstanced(10, myInstanceConstantBuffer);

			emissiveTeapot->SetLocalRotation(XMVECTOR{ -2,-1,0,1 }, cb, myConstantBuffer, xTimer.TotalTime());
			//emissiveTeapot->RenderIndexedEmissive(myTextureEmissive);

			plant->SetPosition(XMVECTOR{ 3,0,-3,0 }, cb, myConstantBuffer);
			plant->RenderIndexed();

			textureRenderer->MoveCamera(cb, myConstantBuffer, myContext);
			textureRenderer->Clear(myContext, nullptr, XMFLOAT4(1, 1, 1, 1));
			textureRenderer->BeginRender(myContext, mainTextureRenderer->pRenderTargetView);
#if SPACESHIP
			spaceShip->SetLocalRotation(XMVECTOR{ 100,0,0,0 }, cb, myConstantBuffer,(float)xTimer.TotalTime(), (float)xTimer.TotalTime());
			spaceShip->RenderIndexed();
#endif 
			textureRenderer->EndRender(myContext, myRenderTargetView, myDepthStencilView);

			UpdateCamera();

			spaceShipRTT->UpdateTexture(textureRenderer->pCTexture);
			spaceShipRTT->SetPosition(XMVECTOR{ 5, 2, -3, 1 }, cb, myConstantBuffer);
			//spaceShipRTT->RenderIndexedWithDynamicSRV(textureRenderer->pCTexture);

			myContext->OMSetBlendState(transparencyBlendState, blendFactor, 0xffffffff);

			transparentObjects[0]->SetLocalRotation(XMVECTOR{ 2.5,-1.5,0 }, cb, myConstantBuffer, sin(xTimer.TotalTime()));
			transparentObjects[1]->SetLocalRotation(XMVECTOR{ 0,-1.5,0 }, cb, myConstantBuffer, sin(xTimer.TotalTime()));
			transparentObjects[2]->SetLocalRotation(XMVECTOR{ -2.5,-1.5,3 }, cb, myConstantBuffer, sin(xTimer.TotalTime()));
			

			//RenderTransparentObjects();
			myContext->OMSetBlendState(0, 0, 0xffffffff);

			mainTextureRenderer->EndRender(myContext, myRenderTargetView, myDepthStencilView);

			screenQuad->UpdateTexture(mainTextureRenderer->pCTexture);
			screenQuad->Render();


			mainTextureRenderer->pResView = { nullptr };
			myContext->PSSetShaderResources(0, 1, &mainTextureRenderer->pResView.p);

			//Start rendering the right top of the screen
			//TODO: change all light buffers

			rightTopRTT->Clear(myContext, myDepthStencilView, XMFLOAT4(0, 0, 0, 1));
			rightTopRTT->BeginRender(myContext, myRenderTargetView);

			choppedWood->SetLocalRotation(XMVECTOR{ -1,0,0 }, cb, myConstantBuffer,XMConvertToRadians(180.0f));
			choppedWood->RenderIndexed();

			closedSack->SetPosition(XMVECTOR{ 0,0,0.2f }, cb, myConstantBuffer);
			closedSack->RenderIndexed();

			rightTopRTT->EndRender(myContext, myRenderTargetView, myDepthStencilView);

			screenQuadRightTop->UpdateTexture(rightTopRTT->pCTexture);
			screenQuadRightTop->Render();
			rightTopRTT->pResView = { nullptr };
			myContext->PSSetShaderResources(0, 1, &mainTextureRenderer->pResView.p);
			
			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display
			myRenderTargetView.Release(); // Free any temp DX handles aquired this frame  

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
		//projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		fov -= (10 * (float)xTimer.Delta());
		//projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, 0.01f, 100.0f);
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
		camPitch -= 1 * (float)xTimer.Delta();
	}
	else if (GetAsyncKeyState('K'))
	{
		camPitch += 1 * (float)xTimer.Delta();
	}

	if (GetAsyncKeyState('J'))
	{
		camYaw -= 1 * (float)xTimer.Delta();
	}
	else if (GetAsyncKeyState('L'))
	{
		camYaw += 1 * (float)xTimer.Delta();
	}
#pragma endregion

	if (GetAsyncKeyState('N'))
	{
		if (nearPlane > 0.1f)
		{
			nearPlane -= xTimer.Delta();
		}
	}
	else if (GetAsyncKeyState('M'))
	{
		nearPlane += xTimer.Delta();
	}

	if (GetAsyncKeyState('C'))
	{
		if (farPlane > 0.1f)
		{
			farPlane -= xTimer.Delta()*5;
		}
	}
	else if (GetAsyncKeyState('V'))
	{
		farPlane += xTimer.Delta();
	}

	viewMatrix = XMMatrixInverse(0, viewMatrix);
	viewMatrix = XMMatrixTranslationFromVector(XMVECTOR{ 0,0,moveZ })*viewMatrix;
	viewMatrix = XMMatrixTranslationFromVector(XMVECTOR{ moveX,0,0 })*viewMatrix;

	viewMatrix = viewMatrix * XMMatrixTranslationFromVector(XMVECTOR{ 0,moveY,0 });
	viewMatrix = XMMatrixRotationX(camPitch)*viewMatrix;

	XMVECTOR tempPosition = viewMatrix.r[3];
	viewMatrix.r[3] = XMVECTOR{ 0,0,0 };
	viewMatrix = viewMatrix * XMMatrixRotationY(camYaw);
	viewMatrix.r[3] = tempPosition;

	Eye = viewMatrix.r[3];
	At = viewMatrix.r[2];
	Up = viewMatrix.r[1];

	viewMatrix = XMMatrixInverse(0, viewMatrix);

	/////Old Code/////

	//XMMATRIX camRotationMatrix = XMMatrixTranspose(XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0));
	//At = XMVector3TransformCoord(FORWARD, camRotationMatrix);
	//At = XMVector3Normalize(At);

	//XMMATRIX tempRotationMatrix = XMMatrixRotationY(camYaw);


	//XMVECTOR camRight = XMVector3TransformCoord(RIGHT, tempRotationMatrix);
	//Up = XMVector3TransformCoord(Up, tempRotationMatrix);
	//XMVECTOR camForward = XMVector3TransformCoord(FORWARD, tempRotationMatrix);

	//Eye += moveX * camRight;
	//Eye += moveZ * camForward;
	//Eye += moveY * Up;

	///////Old Code//////////

	moveX = 0.0f;
	moveZ = 0.0f;
	moveY = 0;
	camPitch = 0;
	camYaw = 0;
	camRoll = 0;

	float aspectRatio;
	mySurface->GetAspectRatio(aspectRatio);
	projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, nearPlane, farPlane);

	//viewMatrix = XMMatrixLookAtLH(Eye, At, Up);
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
	hr = myDevice->CreateVertexShader(VS_Main, sizeof(VS_Main), nullptr, &myVertexShader);
	hr = myDevice->CreateVertexShader(VS_UVModifier, sizeof(VS_UVModifier), nullptr, &myVertexShaderUV);
	hr = myDevice->CreateVertexShader(VS_SkyBox, sizeof(VS_SkyBox), nullptr, &myVertexShaderSkyBox);
	hr = myDevice->CreateVertexShader(VS_Instance, sizeof(VS_Instance), nullptr, &myVertexShaderInstance);
	hr = myDevice->CreateVertexShader(VS_PositionModifier, sizeof(VS_PositionModifier), nullptr, &myVertexShaderWave);
	hr = myDevice->CreateVertexShader(VS_PassThrough, sizeof(VS_PassThrough), nullptr, &myVertexShaderPassThrough);
	hr = myDevice->CreateVertexShader(VS_Reflective, sizeof(VS_Reflective), nullptr, &myVertexShaderReflective);
	hr = myDevice->CreateVertexShader(VS_ScreenSpace, sizeof(VS_ScreenSpace), nullptr, &myVertexShaderScreenSpace);

	hr = myDevice->CreatePixelShader(PS_Main, sizeof(PS_Main), nullptr, &myPixelShader);
	hr = myDevice->CreatePixelShader(PS_SolidColor, sizeof(PS_SolidColor), nullptr, &myPixelShaderSolid);
	hr = myDevice->CreatePixelShader(PS_SkyBox, sizeof(PS_SkyBox), nullptr, &myPixelShaderSkyBox);
	hr = myDevice->CreatePixelShader(PS_Multitexturing, sizeof(PS_Multitexturing), nullptr, &myPixelShaderMultitexturing);
	hr = myDevice->CreatePixelShader(PS_NoLighting, sizeof(PS_NoLighting), nullptr, &myPixelShaderNoLighting);
	hr = myDevice->CreatePixelShader(PS_Reflective, sizeof(PS_Reflective), nullptr, &myPixelShaderReflective);
	hr = myDevice->CreatePixelShader(PS_PostProcessing, sizeof(PS_PostProcessing), nullptr, &myPixelShaderPostProcessing);
	hr = myDevice->CreatePixelShader(PS_Specular, sizeof(PS_Specular), nullptr, &myPixelShaderSpecular);
	hr = myDevice->CreatePixelShader(PS_Transparent, sizeof(PS_Transparent), nullptr, &myPixelShaderTransparent);
	hr = myDevice->CreatePixelShader(PS_Emissive, sizeof(PS_Emissive), nullptr, &myPixelShaderEmissive);
	hr = myDevice->CreatePixelShader(PS_TransparentRejector, sizeof(PS_TransparentRejector), nullptr, &myPixelShaderTransparentRejector);

	hr = myDevice->CreateGeometryShader(GS_PointToQuad, sizeof(GS_PointToQuad), nullptr, &myGeometryShaderPoint);
	hr = myDevice->CreateGeometryShader(GS_Instancer, sizeof(GS_Instancer), nullptr, &myGeometryShaderTriangle);
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
	myDevice->CreateInputLayout(layout, numElements, VS_Main, sizeof(VS_Main), &myVertexLayout);
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
	projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, nearPlane, farPlane);
}

void LetsDrawSomeStuff::UpdateCamera()
{
	cb.mView = XMMatrixTranspose(viewMatrix);
	myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);
}

void LetsDrawSomeStuff::CreateBlendState()
{
	//Define the Blending Equation
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	myDevice->CreateBlendState(&blendDesc, &transparencyBlendState);
}

void LetsDrawSomeStuff::RenderTransparentObjects()
{
	for (int i = 0; i < transparentObjects.size(); i++)
	{
		transparentObjects[i]->GetDistanceFromCamera(Eye);
	}
	//TODO: Sort using custome algorathim
	//sort(transparentObjects.begin(), transparentObjects.end(), wayToSort);
	for (int i = 0; i < transparentObjects.size(); i++)
	{
		transparentObjects[i]->SetLocalRotation(transparentObjects[i]->GetPosition(), cb, myConstantBuffer, sin(xTimer.TotalTime()));
		transparentObjects[i]->RenderIndexedTransparent();
	}
}