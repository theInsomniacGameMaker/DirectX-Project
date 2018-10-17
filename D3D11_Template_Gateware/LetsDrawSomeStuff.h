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

	CURRENT_RENDERING currentRendering;
	int currentRenderingIndex=1;

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
	CComPtr<ID3D11VertexShader>				myVertexShaderTBN = nullptr;
	CComPtr<ID3D11VertexShader>				myVertexShaderInstanceTBN = nullptr;

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
	CComPtr<ID3D11PixelShader>				myPixelShaderAO = nullptr;
	CComPtr<ID3D11PixelShader>				myPixelShaderNormalMapping = nullptr;

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
	ScreenQuad*		screenQuadLeftTop;

	//RTT variables
	TextureRenderer*	textureRenderer;
	TextureRenderer*	leftTopRTT;

	//All 3D Objects 
	D3DObject*		feraligtr;
	D3DObject*		well;
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

	D3DObject*		humvee;
	D3DObject*		desert_house1;
	D3DObject*		desert_house2;
	D3DObject*		desert_palmtree1;
	D3DObject*		desert_palmtree2;
	D3DObject*		desert_containerRed;
	D3DObject*		desert_containerBlue;
	D3DObject*		desert_containerGreen;
	D3DObject*		desert_well;
	D3DObject*		desert_barrel;
	D3DObject*		desert_light;
	D3DObject*		desert_tireWall;
	D3DObject*		desert_pressureTank;
	D3DObject*		desert_barrier;
	D3DObject*		desert_crate;
	D3DObject*		desert_ground;
	D3DObject*		desert_humvee;

	D3DObject*		space_Sun;
	D3DObject*		space_planetMercury;
	D3DObject*		space_planetVenus;
	D3DObject*		space_planetEarth;
	D3DObject*		space_moon;
	D3DObject*		space_planetMars;
	D3DObject*		space_planetJupiter;
	D3DObject*		space_planetSaturnRing;
	D3DObject*		space_planetSaturn;
	D3DObject*		space_planetUranus;
	D3DObject*		space_planetNeptune;
	D3DObject*		space_satellite;

	D3DObject*		pokemon_xerneas;
	D3DObject*		pokemon_metagross;
	D3DObject*		pokemon_darkrai;
	D3DObject*		pokemon_kyurem;
	D3DObject*		pokemon_ground;
	D3DObject*		pokemon_gogoat;
	D3DObject*		med_table;

	//Constant Buffers
	InstanceConstantBuffer	iCb;
	LightConstantBuffer		lCb;
	ConstantBuffer			cb;


	float A, B, C, D;

	//The vector that contains all the transparent objects
	vector<D3DObject*>		transparentObjects;
	//Timer class
	XTime					xTimer;

	float camYaw, camPitch, camRoll;
	float moveX, moveY, moveZ;
	float fov = 60;
	float nearPlane = 0.01f, farPlane = 150.0f;
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
	void Render(GW::SYSTEM::GWindow* attatchPoint);
	//Camera Movement
	void CameraMovement();
	//Reset Camera
	void ResetCamera();
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
	//Desert Scene
	void RenderDesertScene();
	//Space Scene
	void RenderSpaceScene();
	//Pokemon Scene
	void RenderPokemonScene();
	//Key Input
	void GetKeyInput();


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
			currentRendering = (CURRENT_RENDERING)(currentRenderingIndex % 5);

			CreateInputLayout();
#pragma region SANDBOX_INIT

			transparentObjects.resize(3);

			well = new D3DObject("Well", 1 / 65.0f, myDevice, myContext, myVertexShader/*TBN*/, myPixelShader/*NormalMapping*/, nullGeometryShader,
				myConstantBuffer, "Well_Albedo", "Well_Normal", NORMALMAP);
			well->ComputeNormalMapping();

			feraligtr = new D3DObject("Charizard", 5.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular, nullGeometryShader,
				myConstantBuffer);

			skyBox = new D3DObject("SkyBox", 10.0f, myDevice, myContext, myVertexShaderSkyBox, myPixelShaderSkyBox, nullGeometryShader,
				myConstantBuffer);
			skyBox->UpdateTexture("OutputCube");

			ground = new D3DObject("Ground", 10.0f, myDevice, myContext, myVertexShaderWave, myPixelShader, nullGeometryShader,
				myConstantBuffer);

			box = new D3DObject("cube", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderMultitexturing, nullGeometryShader,
				myConstantBuffer);

			bulb = new D3DObject("Bulb", 1.0f / 5, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader, myConstantBuffer);

			spaceShip = new D3DObject("Galaga Fighter", 1.0f / 4, myDevice, myContext, myVertexShader, myPixelShader, nullGeometryShader,
				myConstantBuffer);

			cubeGS = new D3DObject("Q", 5.0f, myDevice, myContext, myVertexShaderPassThrough, myPixelShaderNoLighting, myGeometryShaderPoint,
				myConstantBuffer);

			spaceShipRTT = new D3DObject("cube", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderNoLighting, nullGeometryShader,
				myConstantBuffer);

			reflectiveTeapot = new D3DObject("utah-teapot", 0.1f, myDevice, myContext, myVertexShaderReflective, myPixelShaderReflective,
				nullGeometryShader, myConstantBuffer);
			reflectiveTeapot->UpdateTexture("OutputCube");

			emissiveTeapot = new D3DObject("cube", 1 / 50.0f, myDevice, myContext, myVertexShader,
				myPixelShaderEmissive, nullGeometryShader, myConstantBuffer);
			emissiveTeapot->UpdateTexture("Lava");

			humvee = new D3DObject("Humvee", 1 / 50.0f, myDevice, myContext, myVertexShader,
				myPixelShader, nullGeometryShader, myConstantBuffer, "Humvee_Albedo",
				"Humvee_Occlusion", AMBIENTOCCULUSION);

			plant = new D3DObject("Parviflora", 1.0f / 20.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparentRejector, nullGeometryShader, myConstantBuffer);
			plant->UpdateTexture("Parviflora_diffuse");

			transparentObjects[0] = new D3DObject("cube", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparent, nullGeometryShader, myConstantBuffer);
			transparentObjects[1] = new D3DObject("cube", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparent, nullGeometryShader, myConstantBuffer);
			transparentObjects[2] = new D3DObject("cube", 1 / 50.0f, myDevice, myContext, myVertexShader, myPixelShaderTransparent, nullGeometryShader, myConstantBuffer);

			transparentObjects[0]->UpdateTexture("Energy");
			transparentObjects[1]->UpdateTexture("Energy1");
			transparentObjects[2]->UpdateTexture("Energy");

#pragma endregion

#pragma region DESERT_INIT

			desert_house1 = new D3DObject("House", 1 / 10.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShader, nullGeometryShader, myConstantBuffer, "House_Albedo");

			desert_house2 = new D3DObject("House2", 1 / 10.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShader, nullGeometryShader, myConstantBuffer, "House_Albedo");

			desert_palmtree2 = new D3DObject("PalmTree2", 1 / 15.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShaderTransparentRejector, nullGeometryShader, myConstantBuffer, "Palm_Tree_Albedo");

			desert_palmtree1 = new D3DObject("PalmTree", 1 / 15.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShaderTransparentRejector, nullGeometryShader, myConstantBuffer, "Palm_Tree_Albedo");

			desert_containerRed = new D3DObject("Container", 1 / 35.0f, myDevice, myContext, myVertexShader,
				myPixelShader, nullGeometryShader, myConstantBuffer, "RedContainer_Albedo");

			desert_containerBlue = new D3DObject("Container", 1 / 35.0f, myDevice, myContext, myVertexShader,
				myPixelShader, nullGeometryShader, myConstantBuffer, "BlueContainer_Albedo");

			desert_containerGreen = new D3DObject("Container", 1 / 35.0f, myDevice, myContext, myVertexShader,
				myPixelShader, nullGeometryShader, myConstantBuffer, "GreenContainer_Albedo");

			desert_well = new D3DObject("Well", 1 / 35.0f, myDevice, myContext, myVertexShaderTBN,
				myPixelShaderNormalMapping, nullGeometryShader, myConstantBuffer, "Well_Albedo", "Well_Normal", NORMALMAP);
			desert_well->ComputeNormalMapping();

			desert_barrel = new D3DObject("Barrel", 1, myDevice, myContext, myVertexShader,
				myPixelShaderSpecular, nullGeometryShader, myConstantBuffer, "BlueBarrel_Albedo");

			desert_light = new D3DObject("Light", 1 / 5.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShaderEmissive, nullGeometryShader, myConstantBuffer, "Light_Albedo", "Light_Emissive", EMISSIVE);

			desert_tireWall = new D3DObject("TireWall", 1 / 65.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShader, nullGeometryShader, myConstantBuffer, "TireWall_Albedo");

			desert_pressureTank = new D3DObject("PressureTank", 1 / 65.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShaderSpecular, nullGeometryShader, myConstantBuffer, "PressureTank_Albedo");

			desert_barrier = new D3DObject("ConcreteBarrier", 1 / 65.0f, myDevice, myContext, myVertexShaderInstance,
				myPixelShader, nullGeometryShader, myConstantBuffer, "Barrier_Albedo_BulletHoles");

			desert_crate = new D3DObject("WoodenCrate", 1 / 65.0f, myDevice, myContext, myVertexShaderInstanceTBN,
				myPixelShaderNormalMapping, nullGeometryShader, myConstantBuffer, "WoodenCrate2_Albedo", "WoodenCrate2_Normal", NORMALMAP);
			desert_crate->ComputeNormalMapping();

			desert_ground = new D3DObject("Ground", 100.0f, myDevice, myContext, myVertexShaderTBN,
				myPixelShaderNormalMapping, nullGeometryShader, myConstantBuffer, "Ground_Albedo", "Ground_Normal", NORMALMAP);
			desert_ground->SetHardTangents();

			desert_humvee = new D3DObject("Humvee", 1 / 45.0f, myDevice, myContext, myVertexShader,
				myPixelShaderAO, nullGeometryShader, myConstantBuffer, "Humvee_Albedo",
				"Humvee_Occlusion", AMBIENTOCCULUSION);

#pragma endregion

#pragma region SPACE_INIT
			space_planetMercury = new D3DObject("Planet", 1 / 70.f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "mercurymap");

			space_planetVenus = new D3DObject("Planet", 1 / 55.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "venusmap");

			space_planetEarth = new D3DObject("Planet", 1 / 50.f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "earthmap1k", "earthlights1k", EMISSIVE);

			space_planetMars = new D3DObject("Planet", 1 / 45.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "marsmap");

			space_planetJupiter = new D3DObject("Planet", 1 / 20.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "jupitermap");

			space_planetSaturn = new D3DObject("Planet", 1 / 30.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "saturnmap");

			space_planetSaturnRing = new D3DObject("Planet_Ring", 1 / 30.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "saturnmap");

			space_planetUranus = new D3DObject("Planet", 1 / 35.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "uranusmap");

			space_planetNeptune = new D3DObject("Planet", 1 / 40.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "neptunemap");

			space_moon = new D3DObject("Planet", 1 / 150.0f, myDevice, myContext, myVertexShader, myPixelShader,
				nullGeometryShader, myConstantBuffer, "MoonTexture1", "MoonTexture2", MULTITEXTURE);

			space_satellite = new D3DObject("Satellite", 1 / 220.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular,
				nullGeometryShader, myConstantBuffer, "RT_2D_Station_Diffuse");

			space_Sun = new D3DObject("Planet", 1 / 10.f, myDevice, myContext, myVertexShader, myPixelShaderEmissive,
				nullGeometryShader, myConstantBuffer, "8k_sun", "8k_sun", EMISSIVE);
#pragma endregion


#pragma region POKEMON_INIT
			pokemon_xerneas = new D3DObject("Xerneas", 1 / 70.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular,
				nullGeometryShader, myConstantBuffer, "Xerneas_TX");

			pokemon_metagross = new D3DObject("Metagross", 1 / 30.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular,
				nullGeometryShader, myConstantBuffer, "Metagross_TX");

			pokemon_darkrai = new D3DObject("Darkrai", 1 / 15.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular,
				nullGeometryShader, myConstantBuffer, "Darkrai_TX");

			pokemon_kyurem = new D3DObject("Kyurem", 1 / 35.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular,
				nullGeometryShader, myConstantBuffer, "KyuremTX");

			pokemon_gogoat = new D3DObject("Gogoat", 1 / 35.0f, myDevice, myContext, myVertexShader, myPixelShaderSpecular,
				nullGeometryShader, myConstantBuffer, "GogoatTx");

			pokemon_ground = new D3DObject("Ground", 20.0f, myDevice, myContext, myVertexShaderTBN,
				myPixelShaderNormalMapping, nullGeometryShader, myConstantBuffer, "pokemon_plane", "243-normal", NORMALMAP);
			pokemon_ground->SetHardTangents();


#pragma endregion




			textureRenderer = new TextureRenderer(myDevice, myContext, width, height);
			leftTopRTT = new TextureRenderer(myDevice, myContext, (int)(width / 2.0f), (int)(height / 2.0f));

			XMFLOAT3 positions[4];
			positions[0] = XMFLOAT3(-1, 1, 0);
			positions[1] = XMFLOAT3(1, 1, 0);
			positions[2] = XMFLOAT3(-1, -1, 0);
			positions[3] = XMFLOAT3(1, -1, 0);

			screenQuadLeftTop = new ScreenQuad(myDevice, myContext, myVertexShaderScreenSpace, myPixelShaderPostProcessing, nullGeometryShader, positions);


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
	myPixelShaderEmissive.Release();
	myPixelShaderAO.Release();
	myPixelShaderTransparent.Release();
	myPixelShaderPostProcessing.Release();
	myPixelShaderTransparentRejector.Release();
	myGeometryShaderPoint.Release();
	myGeometryShaderTriangle.Release();

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


	delete leftTopRTT;
	delete textureRenderer;

	delete screenQuadLeftTop;

	delete well;
	delete feraligtr;
	delete skyBox;
	delete ground;
	delete box;
	delete bulb;
	delete spaceShip;
	delete cubeGS;
	delete spaceShipRTT;
	delete reflectiveTeapot;
	delete emissiveTeapot;
	delete secondaryScreen;
	delete plant;

	delete humvee;
	delete desert_house1;
	delete desert_house2;
	delete desert_palmtree1;
	delete desert_palmtree2;
	delete desert_containerRed;
	delete desert_containerBlue;
	delete desert_containerGreen;
	delete desert_well;
	delete desert_barrel;
	delete desert_light;
	delete desert_tireWall;
	delete desert_pressureTank;
	delete desert_barrier;
	delete desert_crate;
	delete desert_ground;
	delete desert_humvee;

	delete space_Sun;
	delete space_planetMercury;
	delete space_planetVenus;
	delete space_planetEarth;
	delete space_planetMars;
	delete space_planetJupiter;
	delete space_planetSaturnRing;
	delete space_planetSaturn;
	delete space_planetUranus;
	delete space_planetNeptune;
	delete space_moon;
	delete space_satellite;

	delete pokemon_xerneas;
	delete pokemon_metagross;
	delete pokemon_darkrai;
	delete pokemon_gogoat;
	delete pokemon_kyurem;
	delete pokemon_ground;

	for (int i = 0; i < transparentObjects.size(); i++)
	{
		delete transparentObjects[i];
	}

	myRenderTargetView.Release();
	myDevice.Release();
	mySwapChain.Release();
	myContext.Release();

}

// Draw
void LetsDrawSomeStuff::Render(GW::SYSTEM::GWindow* attatchPoint)
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
			}

			// Set active target for drawing, all array based D3D11 functions should use a syntax similar to below
			ID3D11RenderTargetView* const targets[] = { myRenderTargetView };
			myContext->OMSetRenderTargets(1, targets, myDepthStencilView);

			// Clear the screen to dark green
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);
			ResetCamera();
			CameraMovement();

			UpdateConstantBuffer();
			UpdateLightBuffer();

#if DEBUGGER
#pragma region DEBUG_CONTROLS
			if (GetAsyncKeyState('T'))
			{
				if (GetAsyncKeyState(VK_LSHIFT))
				{
					A -= (float)xTimer.Delta();
				}
				else
				{
					A += (float)xTimer.Delta();
				}
			}

			if (GetAsyncKeyState('Z'))
			{
				if (GetAsyncKeyState(VK_LSHIFT))
				{
					B -= (float)xTimer.Delta();
				}
				else
				{
					B += (float)xTimer.Delta();
				}
			}

			if (GetAsyncKeyState('Y'))
			{
				if (GetAsyncKeyState(VK_LSHIFT))
				{
					C -= (float)xTimer.Delta();
				}
				else
				{
					C += (float)xTimer.Delta();
				}
			}

			if (GetAsyncKeyState('X'))
			{
				if (GetAsyncKeyState(VK_LSHIFT))
				{
					D -= (float)xTimer.Delta() * 10;
				}
				else
				{
					D += (float)xTimer.Delta() * 10;
				}
			}

			cout << "Current value of A: " << A << endl;
			cout << "Current value of B: " << B << endl;
			cout << "Current value of C: " << C << endl;
			cout << "Current value of D: " << D << endl;
#pragma endregion
#endif


			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer.p);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->PSSetConstantBuffers(0, 1, &myConstantBuffer.p);
			myContext->PSSetConstantBuffers(1, 1, &myLightConstantBuffer.p);
			myContext->PSSetSamplers(0, 1, &mySamplerLinear.p);
			myContext->GSSetShader(nullGeometryShader, nullptr, 0);
			myContext->GSSetShader(myGeometryShaderPoint, nullptr, 0);
			myContext->GSSetConstantBuffers(0, 1, &myConstantBuffer.p);

			GetKeyInput();

			//changing blending factor
			float blendFactor[] = { 0.75f, 0.75f, 0.75f, 1.0f };

			//Set the default blend state (no blending) for opaque objects
			myContext->OMSetBlendState(0, 0, 0xffffffff);

			if (currentRendering == SANDBOX)
			{
				D3D11_VIEWPORT fullscreen;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				fullscreen.Width = (FLOAT)width;
				fullscreen.Height = (FLOAT)height;
				fullscreen.MinDepth = 0.0f;
				fullscreen.MaxDepth = 1.0f;
				fullscreen.TopLeftX = 0;
				fullscreen.TopLeftY = 0;
				myContext->RSSetViewports(1, &fullscreen);


				projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), fullscreen.Width / fullscreen.Height, nearPlane, farPlane);
				cb.mProjection = XMMatrixTranspose(projectionMatrix);
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

				//Render opaque objects//
				skyBox->UpdateTexture("OutputCube");
				skyBox->SetPosition(Eye, cb, myConstantBuffer);
				skyBox->RenderIndexed();
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.


				feraligtr->SetLocalRotation(XMVECTOR{ 5,0,2,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime());
				feraligtr->RenderIndexed();

				ground->SetPosition(XMVECTOR{ 0,-0.5,0,0 }, cb, myConstantBuffer);
				ground->RenderIndexed();

				box->SetPosition(XMVECTOR{ -5,0,0,1 }, cb, myConstantBuffer);
				box->UpdateVS(myVertexShader);
				box->UpdatePS(myPixelShaderMultitexturing);
				box->RenderIndexedMulitexture(myTextureRVPMT);

				reflectiveTeapot->SetLocalRotation(XMVECTOR{ 0,5.0f,0.0f,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime() / 2.0f, (float)xTimer.TotalTime() / 2.0f);
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
				//box->RenderInstanced(10, myInstanceConstantBuffer);

				emissiveTeapot->SetLocalRotation(XMVECTOR{ -2,-1,0,1 }, cb, myConstantBuffer, (float)xTimer.TotalTime());
				emissiveTeapot->RenderIndexedEmissive(myTextureEmissive);

				plant->SetPosition(XMVECTOR{ 3,0,-3,0 }, cb, myConstantBuffer);
				plant->RenderIndexed();

				well->UpdatePS(myPixelShader);
				well->UpdateVS(myVertexShader);
				well->SetPosition(XMVECTOR{ -2,2,0 }, cb, myConstantBuffer);
				well->RenderIndexed();

				well->UpdatePS(myPixelShaderNormalMapping);
				well->UpdateVS(myVertexShaderTBN);
				well->SetPosition(XMVECTOR{ 2,2,0 }, cb, myConstantBuffer);
				well->RenderIndexedNormal();

				humvee->SetLocalRotation(XMVECTOR{ -8,3,0 }, cb, myConstantBuffer, XMConvertToRadians(180.0f));
				humvee->UpdatePS(myPixelShader);
				humvee->RenderIndexed();

				humvee->UpdatePS(myPixelShaderAO);
				humvee->SetLocalRotation(XMVECTOR{ 8,3,0 }, cb, myConstantBuffer, XMConvertToRadians(180.0f));
				humvee->RenderIndexedWithAO();

				textureRenderer->MoveCamera(cb, myConstantBuffer, myContext);
				textureRenderer->Clear(myContext, nullptr, XMFLOAT4(1, 1, 1, 1));
				textureRenderer->BeginRender(myContext, myDevice, (float)width, (float)height);
#if SPACESHIP
				spaceShip->SetLocalRotation(XMVECTOR{ 100,0,0,0 }, cb, myConstantBuffer, (float)xTimer.TotalTime(), (float)xTimer.TotalTime());
				spaceShip->RenderIndexed();
#endif 
				textureRenderer->EndRender();
				myContext->OMSetRenderTargets(1, &myRenderTargetView.p, myDepthStencilView);

				UpdateCamera();

				spaceShipRTT->UpdateTexture(textureRenderer->pCTexture);
				spaceShipRTT->SetPosition(XMVECTOR{ 5, 2, -3, 1 }, cb, myConstantBuffer);
				spaceShipRTT->RenderIndexedWithDynamicSRV(textureRenderer->pCTexture);


				myContext->OMSetBlendState(transparencyBlendState, blendFactor, 0xffffffff);

				transparentObjects[0]->SetLocalRotation(XMVECTOR{ 2.5,-1.5,0 }, cb, myConstantBuffer, (float)sin(xTimer.TotalTime()));
				transparentObjects[1]->SetLocalRotation(XMVECTOR{ 0,-1.5,0 }, cb, myConstantBuffer, (float)sin(xTimer.TotalTime()));
				transparentObjects[2]->SetLocalRotation(XMVECTOR{ -2.5,-1.5,3 }, cb, myConstantBuffer, (float)sin(xTimer.TotalTime()));


				RenderTransparentObjects();
				myContext->OMSetBlendState(0, 0, 0xffffffff);

			}
			else if (currentRendering == MULTIPORT)
			{
				D3D11_VIEWPORT vpTopRight;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				vpTopRight.Width = (FLOAT)width / 2.0f;
				vpTopRight.Height = (FLOAT)height / 2.0f;
				vpTopRight.MinDepth = 0.0f;
				vpTopRight.MaxDepth = 1.0f;
				vpTopRight.TopLeftX = (FLOAT)width / 2.0f;
				vpTopRight.TopLeftY = 0;
				myContext->RSSetViewports(1, &vpTopRight);

				skyBox->UpdateTexture("DesertSkyBox");
				skyBox->SetPosition(Eye, cb, myConstantBuffer);
				skyBox->RenderIndexed();
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

				RenderDesertScene();


				D3D11_VIEWPORT vpBottom;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				vpBottom.Width = (FLOAT)width;
				vpBottom.Height = (FLOAT)height / 2.0f;
				vpBottom.MinDepth = 0.0f;
				vpBottom.MaxDepth = 1.0f;
				vpBottom.TopLeftX = 0;
				vpBottom.TopLeftY = (FLOAT)height / 2.0f;
				myContext->RSSetViewports(1, &vpBottom);


				projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), vpBottom.Width / vpBottom.Height, nearPlane, farPlane);
				cb.mProjection = XMMatrixTranspose(projectionMatrix);
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);


				skyBox->UpdateTexture("StarField");
				skyBox->SetPosition(Eye, cb, myConstantBuffer);
				skyBox->RenderIndexed();
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

				RenderSpaceScene();

				D3D11_VIEWPORT vpTopLeft;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				vpTopLeft.Width = (FLOAT)width / 2.0f;
				vpTopLeft.Height = (FLOAT)height / 2.0f;
				vpTopLeft.MinDepth = 0.0f;
				vpTopLeft.MaxDepth = 1.0f;
				vpTopLeft.TopLeftX = 0;
				vpTopLeft.TopLeftY = 0;

				myContext->RSSetViewports(1, &vpTopLeft);

				projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), vpTopLeft.Width / vpTopLeft.Height, nearPlane, farPlane);
				cb.mProjection = XMMatrixTranspose(projectionMatrix);
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

				leftTopRTT->Clear(myContext, myDepthStencilView, XMFLOAT4(1, 1, 0, 0));
				leftTopRTT->BeginRender(myContext, myDevice, vpTopLeft.Width, vpTopLeft.Height);

				RenderPokemonScene();

				leftTopRTT->EndRender();
				myContext->OMSetRenderTargets(1, &myRenderTargetView.p, myDepthStencilView);


				screenQuadLeftTop->UpdateTexture(leftTopRTT->pCTexture);
				screenQuadLeftTop->Render();
				leftTopRTT->pResView = { nullptr };
				myContext->PSSetShaderResources(0, 1, &leftTopRTT->pResView.p);
			}
			else if (currentRendering == FPS)
			{
				D3D11_VIEWPORT fullscreen;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				fullscreen.Width = (FLOAT)width;
				fullscreen.Height = (FLOAT)height;
				fullscreen.MinDepth = 0.0f;
				fullscreen.MaxDepth = 1.0f;
				fullscreen.TopLeftX = 0;
				fullscreen.TopLeftY = 0;
				myContext->RSSetViewports(1, &fullscreen);

				projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), fullscreen.Width / fullscreen.Height, nearPlane, farPlane);
				cb.mProjection = XMMatrixTranspose(projectionMatrix);
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

				skyBox->UpdateTexture("DesertSkyBox");
				skyBox->SetPosition(Eye, cb, myConstantBuffer);
				skyBox->RenderIndexed();
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

				RenderDesertScene();
			}
			else if (currentRendering == SPACE)
			{
				D3D11_VIEWPORT fullscreen;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				fullscreen.Width = (FLOAT)width;
				fullscreen.Height = (FLOAT)height;
				fullscreen.MinDepth = 0.0f;
				fullscreen.MaxDepth = 1.0f;
				fullscreen.TopLeftX = 0;
				fullscreen.TopLeftY = 0;
				myContext->RSSetViewports(1, &fullscreen);

				projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), fullscreen.Width / fullscreen.Height, nearPlane, farPlane);
				cb.mProjection = XMMatrixTranspose(projectionMatrix);
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

				skyBox->UpdateTexture("StarField");
				skyBox->SetPosition(Eye, cb, myConstantBuffer);
				skyBox->RenderIndexed();
				myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

				RenderSpaceScene();
			}
			else if (currentRendering == POKEMON)
			{

				D3D11_VIEWPORT fullscreen;
				attatchPoint->GetClientWidth(width);
				attatchPoint->GetClientHeight(height);
				fullscreen.Width = (FLOAT)width;
				fullscreen.Height = (FLOAT)height;
				fullscreen.MinDepth = 0.0f;
				fullscreen.MaxDepth = 1.0f;
				fullscreen.TopLeftX = 0;
				fullscreen.TopLeftY = 0;
				myContext->RSSetViewports(1, &fullscreen);

				projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), fullscreen.Width / fullscreen.Height, nearPlane, farPlane);
				cb.mProjection = XMMatrixTranspose(projectionMatrix);
				myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

				leftTopRTT->Clear(myContext, myDepthStencilView, XMFLOAT4(1, 1, 0, 0));
				leftTopRTT->BeginRender(myContext, myDevice, fullscreen.Width, fullscreen.Height);

				RenderPokemonScene();

				leftTopRTT->EndRender();
				myContext->OMSetRenderTargets(1, &myRenderTargetView.p, myDepthStencilView);


				screenQuadLeftTop->UpdateTexture(leftTopRTT->pCTexture);
				screenQuadLeftTop->Render();
				leftTopRTT->pResView = { nullptr };
				myContext->PSSetShaderResources(0, 1, &leftTopRTT->pResView.p);
			}


			// Present Backbuffer using Swapchain object
			// Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display
			myRenderTargetView.Release(); // Free any temp DX handles aquired this frame  
			myDepthStencilView.Release();

		}
	}
}

void LetsDrawSomeStuff::CameraMovement()
{
#pragma region CAMERA_MOVEMENT

	if (GetAsyncKeyState('E'))
	{
		moveY += (float)xTimer.Delta()*15.0f;
	}
	else if (GetAsyncKeyState('Q'))
	{
		moveY -= (float)xTimer.Delta()*15.0f;
	}

	if (GetAsyncKeyState('W'))
	{
		moveZ += (float)xTimer.Delta()*15.0f;
	}
	else if (GetAsyncKeyState('S'))
	{
		moveZ -= (float)xTimer.Delta()*15.0f;
	}

	if (GetAsyncKeyState('D'))
	{
		moveX += (float)xTimer.Delta()*15.0f;
	}
	else if (GetAsyncKeyState('A'))
	{
		moveX -= (float)xTimer.Delta()*15.0f;
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
		camRoll += 1 * (float)xTimer.Delta()*2.0f;
	}
	else if (GetAsyncKeyState('U'))
	{
		camRoll -= 1 * (float)xTimer.Delta()*2.0f;
	}

	if (GetAsyncKeyState('I'))
	{
		camPitch -= 1 * (float)xTimer.Delta()*2.0f;
	}
	else if (GetAsyncKeyState('K'))
	{
		camPitch += 1 * (float)xTimer.Delta()*2.0f;
	}

	if (GetAsyncKeyState('J'))
	{
		camYaw -= 1 * (float)xTimer.Delta()*2.0f;
	}
	else if (GetAsyncKeyState('L'))
	{
		camYaw += 1 * (float)xTimer.Delta()*2.0f;
	}
#pragma endregion

	if (GetAsyncKeyState('N'))
	{
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			if (nearPlane > 0.1f)
			{
				nearPlane -= (float)xTimer.Delta();
			}
		}
		else
		{
			nearPlane += (float)xTimer.Delta();
		}
	}


	if (GetAsyncKeyState('F'))
	{
		if (GetAsyncKeyState(VK_LSHIFT))
		{
			if (farPlane > 1.0f)
			{
				farPlane -= (float)xTimer.Delta()*10.0f;
			}
		}
		else
		{
			farPlane += (float)xTimer.Delta()*10.0f;
		}
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

	moveX = 0.0f;
	moveZ = 0.0f;
	moveY = 0;
	camPitch = 0;
	camYaw = 0;
	camRoll = 0;

	float aspectRatio;
	mySurface->GetAspectRatio(aspectRatio);
	projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, nearPlane, farPlane);

}

inline void LetsDrawSomeStuff::ResetCamera()
{
	if (GetAsyncKeyState('R'))
	{
		// Initialize the view matrix
		Eye = XMVectorSet(0.0f, 1.0f, -10.0f, 0.0f);
		At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

		fov = 60;
		nearPlane = 0.1f;
		farPlane = 150.0f;
		// Initialize the projection matrix
		projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(fov), width / (FLOAT)height, nearPlane, farPlane);
	}
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
	lCb.lights[0].Direction = XMFLOAT4(0.577f, -0.577f, -0.577f, 1.0f);

#if DIRECTIONAL_LIGHT_ON
	lCb.lights[0].Color = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
#else
	lCb.lights[0].Color = XMFLOAT4(0, 0, 0, 0);
#endif

	lCb.lights[1].Position = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[1].Direction = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
#if DIRECTIONAL_LIGHT_ON
	lCb.lights[1].Color = XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f);
	lCb.lights[1].Color = XMFLOAT4(0, 0, 0, 0);
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

	lCb.lights[5].Position.w = 0;
	lCb.lights[5].Color = XMFLOAT4(1, 1, 1, 1);
	lCb.lights[5].Direction.x = 0.2f;


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
	hr = myDevice->CreateVertexShader(VS_TBN, sizeof(VS_TBN), nullptr, &myVertexShaderTBN);
	hr = myDevice->CreateVertexShader(VS_InstanceTBN, sizeof(VS_InstanceTBN), nullptr, &myVertexShaderInstanceTBN);

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
	hr = myDevice->CreatePixelShader(PS_AmbientOcculusion, sizeof(PS_AmbientOcculusion), nullptr, &myPixelShaderAO);
	hr = myDevice->CreatePixelShader(PS_NormalMapping, sizeof(PS_NormalMapping), nullptr, &myPixelShaderNormalMapping);

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

	vector<float> distances(transparentObjects.size());
	vector<int> indices(transparentObjects.size());

	for (int i = 0; i < distances.size(); i++)
	{
		distances[i] = transparentObjects[i]->m_DistanceFromCamera;
		indices[i] = i;
	}

	for (int i = 0; i < distances.size() - 1; i++)
	{
		// Last i elements are already in place    
		for (int j = 0; j < distances.size() - i - 1; j++)
		{
			if (distances[j] < distances[j + 1])
			{
				float tempF = distances[j];
				distances[j] = distances[j + 1];
				distances[j + 1] = tempF;

				int tempI = indices[j];
				indices[j] = indices[j + 1];
				indices[j + 1] = tempI;
			}
		}
	}

	for (int i = 0; i < transparentObjects.size(); i++)
	{
		//transparentObjects[indices[i]]->SetLocalRotation(transparentObjects[i]->GetPosition(), cb, myConstantBuffer, (float)sin(xTimer.TotalTime()));
		transparentObjects[indices[i]]->PositionRenderIndexedTransparent(cb, myConstantBuffer);
	}
}

inline void LetsDrawSomeStuff::RenderDesertScene()
{

	lCb.lights[0].Position = XMFLOAT4(0, 0, 0, 1.0f);
	lCb.lights[0].Direction = XMFLOAT4(-0.577f, 0.577f, -0.577f, 1.0f);
	lCb.lights[0].Color = XMFLOAT4(0.95f / 3.0f, 0.85f / 3.0f, 0.69f / 3.0f, 1);

	lCb.lights[1].Position = XMFLOAT4(-0.8471255f, 1.361888f*4.0f, -1.361888f, 3);
	lCb.lights[1].Direction = XMFLOAT4(0.88f - 0.830f, 0, 0.56f + 4.19f, 10.0f);
	lCb.lights[1].Range.x = 0.9f;
	lCb.lights[1].Range.y = 0.8f;
	lCb.lights[1].Range.z = 70.0f;
	lCb.lights[1].Color = XMFLOAT4(1, 1, 1, 1);

	lCb.lights[2].Position = XMFLOAT4(-12.82835f + 56.25f, 4.0f, 7.0f - 10.55f, 3.0f);
	lCb.lights[2].Direction = XMFLOAT4(-16.0f, 0.0f, -0.208f, 10.0f);
	lCb.lights[2].Range.x = 0.9f;
	lCb.lights[2].Range.y = 0.8f;
	lCb.lights[2].Range.z = 70;
	lCb.lights[2].Color = XMFLOAT4(1, 1, 1, 1);

	lCb.lights[3].Position = XMFLOAT4(1.131f + 3.230f - 11.07f, 0, -18.629f - 23.8819f + 4.38f, 3.0f);
	lCb.lights[3].Direction = XMFLOAT4(3.731f, 0, 3.671f, 10.0f);
	lCb.lights[3].Range.x = 0.9f;
	lCb.lights[3].Range.y = 0.8f;
	lCb.lights[3].Range.z = 70;
	lCb.lights[3].Color = XMFLOAT4(1, 1, 1, 1);

	lCb.lights[4].Color = XMFLOAT4(0, 0, 0, 0);

	lCb.lights[5].Position.w = 0;
	lCb.lights[5].Color = XMFLOAT4(1, 1, 1, 1);
	lCb.lights[5].Direction.x = 0.15f;

	myContext->UpdateSubresource(myLightConstantBuffer, 0, nullptr, &lCb, 0, 0);


	//myContext->ClearDepthStencilView(myDepthStencilView, D3D11_CLEAR_DEPTH, 1, 0); // clear it to Z exponential Far.

	desert_ground->SetPosition(XMVECTOR{ 0,0,0,0 }, cb, myConstantBuffer);
	desert_ground->RenderIndexedNormal();

	iCb.worldArray[0] = MakeWorldMatrix(-3.46f - 31.8729f + 19.3f, 0.0f, 5.7f + 33.86f + 1.88f, 0, -90.0f + 39.0f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(-17.0f, 0, -3.1f, 0, 72.0f, 0);
	iCb.worldArray[2] = MakeWorldMatrix(19.17f, 0, 5.09f, 0, 72.0f, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_house1->RenderInstanced(3, myInstanceConstantBuffer);

	//Rendering Palm Tree 1
	iCb.worldArray[0] = MakeWorldMatrix(-17.6f, 0, -26.4f, 16.0f, 65.0f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(3.9f + 8.18f, 0, -12.0f - 3.38f, 16.0f, 72.0f, 0);
	iCb.worldArray[2] = MakeWorldMatrix(-8.0f, 0, 4.65f, 16.0f, 72.0f, 0);
	iCb.worldArray[3] = MakeWorldMatrix(7.7f, 0, -33.0f, 16.0f, 125.28f, 0);
	iCb.worldArray[4] = MakeWorldMatrix(-13.73f, 0.0f, 32.23f, 16.0f, 125.28f, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);

	desert_palmtree2->RenderInstanced(5, myInstanceConstantBuffer);


	//Rendering Palm Tree 2
	iCb.worldArray[0] = MakeWorldMatrix(-10.6f, 0, -43, 0, 152, 0);
	iCb.worldArray[1] = MakeWorldMatrix(3.9f, 0, -39, 0, 152, 0);
	iCb.worldArray[2] = MakeWorldMatrix(20 + 11.62f, 0, -1.5f - 30.29f, 0, 152, 0);
	iCb.worldArray[3] = MakeWorldMatrix(10.0f + 21.3116f, 0, -8.0f + 42.9204f, 0, 152.0f, 0);
	iCb.worldArray[4] = MakeWorldMatrix(0.6f + 40.6197f, 0, -16.0f, 0, 152.0f, 0);
	iCb.worldArray[5] = MakeWorldMatrix(-0.6f + 31.7339f, 0, -4 + 16.8939f, 0, 152.0f, 0);
	iCb.worldArray[6] = MakeWorldMatrix(-24.6f, 0, -9.0f, 0, 152.0f, 0);
	iCb.worldArray[7] = MakeWorldMatrix(-4.56f, 0, -6.0f, 0, 152.0f, 0);
	iCb.worldArray[8] = MakeWorldMatrix(-18.0f, 0, -8.0f, 0, 152.0f, 0);
	iCb.worldArray[9] = MakeWorldMatrix(-7.6f - 22.5094f, 0, -19.0f + 46.39f, 0, 152.0f, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_palmtree1->RenderInstanced(10, myInstanceConstantBuffer);

	//Rendering House2
	iCb.worldArray[0] = MakeWorldMatrix(7.72f*2.0f + 11.211f, 0, -12 * 2.0f + 4.010f, 0, 90 + 27.0f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(3.9f*3.2f, 0, 21 + 6.62f, 0, 37, 0);
	iCb.worldArray[2] = MakeWorldMatrix(0.32f*1.75f, 0, -18.94f*0.8f, 0, 109, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_house2->RenderInstanced(3, myInstanceConstantBuffer);

	desert_containerRed->SetLocalRotation(XMVECTOR{ -13.31f - 6.28f + 0.1187f,3.18f - 8.4151f,12.44f + 11.60f - 6.334f }, cb, myConstantBuffer, XMConvertToRadians(76.61f));
	desert_containerRed->RenderIndexed();

	desert_containerBlue->SetLocalRotation(XMVECTOR{ -22.83f,-16.0f + 3.51f,14.04f }, cb, myConstantBuffer, XMConvertToRadians(126.4f));
	desert_containerBlue->RenderIndexed();

	desert_containerGreen->SetLocalRotation(XMVECTOR{ -9.93f - 5.841f,-16.0f + 3.51f,13.7f + 7.8384f }, cb, myConstantBuffer, XMConvertToRadians(151.615f));
	desert_containerGreen->RenderIndexed();

	desert_well->SetLocalRotation(XMVECTOR{ -6.112831f + 5.8945f,0,-11.38f - 17.792f }, cb, myConstantBuffer, 0, 90, 0);
	desert_well->RenderIndexedNormal();

	desert_barrel->SetPosition(XMVECTOR{ -11.70f,1.5f,-17.49f }, cb, myConstantBuffer);
	desert_barrel->RenderIndexed();

	desert_barrel->SetPosition(XMVECTOR{ 4.118042f,1.5f,9.440564f }, cb, myConstantBuffer);
	desert_barrel->RenderIndexed();
	desert_barrel->UpdateTexture("RedBarrel_Albedo");

	desert_barrel->SetPosition(XMVECTOR{ -4.359656f, 1.5f,8.526803f }, cb, myConstantBuffer);
	desert_barrel->RenderIndexed();

	desert_barrel->SetLocalRotation(XMVECTOR{ 1.32f - 2, 1.5f + 7.5f,-18.034f + 6 }, cb, myConstantBuffer, 90.0f);
	desert_barrel->RenderIndexed();

	iCb.worldArray[0] = MakeWorldMatrix(-0.8471255f, 0, -1.361888f, 0, -95.97501f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(-12.82835f + 56.25f, 0, 7.0f - 10.55f, 0, -150 - 26.774f, 0);
	iCb.worldArray[2] = MakeWorldMatrix(1.131f + 3.230f - 11.07f, 0, -18.629f - 23.8819f + 4.38f, 0, -213.068f + 129.22f + 36.62f, 0);
	iCb.worldArray[3] = MakeWorldMatrix(12.0f, 0, -25.33f, 0, 0, 0);
	iCb.worldArray[4] = MakeWorldMatrix(-12.69f, 0, -24, 0, 0, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_light->RenderIndexedEmissiveInstanced(3, myInstanceConstantBuffer);

	iCb.worldArray[0] = MakeWorldMatrix(1.985f + 9.946f, 0, -5.46f + 21.096f, 0, 24.397f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(3.55f + 26.7051f, 0, -12.83f + 2.10f, 0, -5.603f + 117.6f, 0);
	iCb.worldArray[2] = MakeWorldMatrix(-13.78f, 0, -10, 0, -88, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_tireWall->RenderIndexedEmissiveInstanced(3, myInstanceConstantBuffer);

	iCb.worldArray[0] = MakeWorldMatrix(-12.61f, 0, -1.656f, 0, -18.7f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(19.4452f, 5.968f, 0.9f, 0, -15.1244f, 0);
	iCb.worldArray[2] = MakeWorldMatrix(23.51f, 5.96f, -23.66f, 0, 29.47f, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_pressureTank->RenderInstanced(3, myInstanceConstantBuffer);

	iCb.worldArray[0] = MakeWorldMatrix(0, 0, 42.8f, 0, 0, 0);
	iCb.worldArray[1] = MakeWorldMatrix(5.89f, 0, 41.5f, 0, 25, 0);
	iCb.worldArray[2] = MakeWorldMatrix(14.74f, 0, -11.0f, 0, -27.536f, 0);
	iCb.worldArray[3] = MakeWorldMatrix(-19.28f, 0, -18.37f, 0, 61.123f, 0);
	iCb.worldArray[4] = MakeWorldMatrix(24.89f, 2.723f, 18.91f, 94.3f + 10.781f, 76.03f + 180, 0);
	iCb.worldArray[5] = MakeWorldMatrix(-1.72f, 0, 26.07f, 0, 0, 0);
	iCb.worldArray[6] = MakeWorldMatrix(-1.72f - 7.0f, 0, 26.07f - 1.87f, 0, -38.2752f, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_barrier->RenderInstanced(7, myInstanceConstantBuffer);


	iCb.worldArray[0] = MakeWorldMatrix(6.4f, 0, 23.5f, 0, 32.5f, 0);
	iCb.worldArray[1] = MakeWorldMatrix(6.4f + 1.39f, 0, 23.5f - 0.85f, 0, 32.5f + 3.28f, 0);
	iCb.worldArray[2] = MakeWorldMatrix(6.4f + 0.6190f, 0 + 1.57f, 23.5f - 0.24f, 0, 32.5f, 0);
	iCb.worldArray[3] = MakeWorldMatrix(-5.189f, 0, -11.42f, 0, 18.028f, 0);
	myContext->UpdateSubresource(myInstanceConstantBuffer, 0, nullptr, &iCb, 0, 0);
	desert_crate->RenderIndexedInstancedNormal(4, myInstanceConstantBuffer);

	desert_humvee->SetLocalRotation(XMVECTOR{ -1.79f,0,36.59f }, cb, myConstantBuffer, 9.73f);
	desert_humvee->RenderIndexedWithAO();
}

inline void LetsDrawSomeStuff::RenderSpaceScene()
{

	lCb.lights[0].Color = XMFLOAT4(0, 0, 0, 0);
	lCb.lights[1].Color = XMFLOAT4(0, 0, 0, 0);
	lCb.lights[2].Color = XMFLOAT4(0, 0, 0, 0);
	lCb.lights[3].Color = XMFLOAT4(0, 0, 0, 0);
	lCb.lights[4].Color = XMFLOAT4(0, 0, 0, 0);

	lCb.lights[0].Position = XMFLOAT4(0, 0, 0, 2);
	lCb.lights[0].Color = XMFLOAT4(0.95f, 0.85f, 0.69f, 1);
	lCb.lights[0].Range.x = 100.0f;

	lCb.lights[5].Direction.x = 0.01f;
	myContext->UpdateSubresource(myLightConstantBuffer, 0, nullptr, &lCb, 0, 0);



	space_Sun->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 0, 0, (float)xTimer.TotalTime(), 0)),
		cb, myConstantBuffer);
	space_Sun->RenderIndexedEmissive();

	space_planetMercury->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 7.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.47f), cb, myConstantBuffer);
	space_planetMercury->RenderIndexed();

	space_planetVenus->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 12.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.32f), cb, myConstantBuffer);
	space_planetVenus->RenderIndexed();

	space_planetEarth->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 18, 0, (float)xTimer.TotalTime() * 50, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.3f), cb, myConstantBuffer);
	space_planetEarth->RenderIndexedEmissive();


	XMMATRIX moonWorldMatrix = XMMatrixIdentity();
	moonWorldMatrix = XMMatrixRotationY((float)xTimer.TotalTime()) *moonWorldMatrix;
	moonWorldMatrix = moonWorldMatrix * XMMatrixTranslation(space_planetEarth->GetPosition().m128_f32[0],
		space_planetEarth->GetPosition().m128_f32[1],
		space_planetEarth->GetPosition().m128_f32[2]);

	moonWorldMatrix = XMMatrixTranslation(0, 0, 2.5)*moonWorldMatrix;
	moonWorldMatrix = XMMatrixRotationY((float)xTimer.TotalTime() / 10.0f) *moonWorldMatrix;

	space_moon->SetPosition(moonWorldMatrix, cb, myConstantBuffer);
	space_moon->RenderIndexedMulitexture();


	space_planetMars->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 25.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.24f), cb, myConstantBuffer);
	space_planetMars->RenderIndexed();

	space_planetJupiter->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 32.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.13f), cb, myConstantBuffer);
	space_planetJupiter->RenderIndexed();

	space_planetSaturn->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 39.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.1f), cb, myConstantBuffer);
	space_planetSaturn->RenderIndexed();

	space_planetSaturnRing->SetPosition(space_planetSaturn->GetPosition(), cb, myConstantBuffer);
	space_planetSaturnRing->RenderIndexed();

	space_planetUranus->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 46.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.07f), cb, myConstantBuffer);
	space_planetUranus->RenderIndexed();

	space_planetNeptune->SetPosition(XMMatrixTranspose(MakeWorldMatrix(0, 0, 55.0f, 0, (float)xTimer.TotalTime() * 80, 0))
		*XMMatrixRotationY((float)-xTimer.TotalTime() * 0.05f), cb, myConstantBuffer);
	space_planetNeptune->RenderIndexed();



	XMMATRIX satelliteWorldMatrix = XMMatrixIdentity();
	satelliteWorldMatrix = XMMatrixRotationY((float)xTimer.TotalTime()) *satelliteWorldMatrix;
	satelliteWorldMatrix = satelliteWorldMatrix * XMMatrixTranslation(space_planetEarth->GetPosition().m128_f32[0],
		space_planetEarth->GetPosition().m128_f32[1],
		space_planetEarth->GetPosition().m128_f32[2]);

	satelliteWorldMatrix = XMMatrixTranslation(1.0f, 0, 1.0f)*satelliteWorldMatrix;
	satelliteWorldMatrix = XMMatrixRotationY((float)xTimer.TotalTime() / 5.0f) *satelliteWorldMatrix;
	satelliteWorldMatrix = XMMatrixRotationX((float)xTimer.TotalTime() / 10.0f) *satelliteWorldMatrix;

	space_satellite->SetPosition(satelliteWorldMatrix, cb, myConstantBuffer);
	space_satellite->RenderIndexed();

}

inline void LetsDrawSomeStuff::RenderPokemonScene()
{

	lCb.lights[0].Color = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[1].Color = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[2].Color = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[3].Color = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[4].Color = XMFLOAT4(0, 0, 0, 1);
	lCb.lights[5].Color = XMFLOAT4(0, 0, 0, 1);

	lCb.lights[0].Position = XMFLOAT4(sin((float)xTimer.TotalTime()) * 5, 2, cos((float)xTimer.TotalTime()) * 5, 2);
	lCb.lights[0].Color = XMFLOAT4(1, 0, 1, 1);
	lCb.lights[0].Range.x = 2.7f + A;

	lCb.lights[1].Position = XMFLOAT4(sin((float)xTimer.TotalTime()) * 3, 5, -cos((float)xTimer.TotalTime()) * 3, 3);
	lCb.lights[1].Direction = XMFLOAT4(0, -1, 0, 0);
	lCb.lights[1].Range.x = 0.9f;
	lCb.lights[1].Range.y = 0.8f;
	lCb.lights[1].Range.z = 10;
	lCb.lights[1].Color = XMFLOAT4(0.47f, 0.1944f, 0.33f, 1);

	lCb.lights[2].Position = XMFLOAT4(-sin((float)xTimer.TotalTime()) * 4, 2, -cos((float)xTimer.TotalTime()) * 4, 2);
	lCb.lights[2].Color = XMFLOAT4(0, 1, 1, 1);
	lCb.lights[2].Range.x = 2.7f + A;

	lCb.lights[3].Position = XMFLOAT4((float)sin((float)xTimer.TotalTime())*0.5f, 2, cos((float)xTimer.TotalTime())*0.5f, 2);
	lCb.lights[3].Color = XMFLOAT4(1, 1, 0, 1);
	lCb.lights[3].Range.x = 2.7f + A;

	myContext->UpdateSubresource(myLightConstantBuffer, 0, nullptr, &lCb, 0, 0);




	skyBox->UpdateTexture("Pokemon_SkyBox");
	skyBox->SetPosition(Eye, cb, myConstantBuffer);
	skyBox->RenderIndexed();
	leftTopRTT->ClearDPV(myContext);

	pokemon_xerneas->SetLocalRotation(XMVECTOR{ 0,0,0 }, cb, myConstantBuffer, XMConvertToRadians(180.0f));
	pokemon_xerneas->RenderIndexed();

	pokemon_metagross->SetLocalRotation(XMVECTOR{ 2,0,0 }, cb, myConstantBuffer, XMConvertToRadians(210.0f));
	pokemon_metagross->RenderIndexed();

	pokemon_darkrai->SetLocalRotation(XMVECTOR{ -2,0,0 }, cb, myConstantBuffer, XMConvertToRadians(-210.0f));
	pokemon_darkrai->RenderIndexed();

	pokemon_kyurem->SetLocalRotation(XMVECTOR{ -4.5f,0,0 }, cb, myConstantBuffer, XMConvertToRadians(-230.0f));
	pokemon_kyurem->RenderIndexed();

	pokemon_gogoat->SetLocalRotation(XMVECTOR{ 4.5f,0,0 }, cb, myConstantBuffer, XMConvertToRadians(230.0f));
	pokemon_gogoat->RenderIndexed();


	pokemon_ground->SetLocalRotation(XMVECTOR{ 0,0,0 }, cb, myConstantBuffer, XMConvertToRadians(0));
	pokemon_ground->RenderIndexedNormal();




}

inline void LetsDrawSomeStuff::GetKeyInput()
{
	if (GetAsyncKeyState('P') & 0x1)
	{
		currentRenderingIndex++;
		currentRendering = (CURRENT_RENDERING)(currentRenderingIndex % 5);
	}
}
