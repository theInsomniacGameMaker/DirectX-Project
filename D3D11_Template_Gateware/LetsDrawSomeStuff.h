// This file can be used to contain very basic DX11 Loading, Drawing & Clean Up. (Start Here, or Make your own set of classes)
#pragma once
// Include our DX11 middle ware
#include "Gateware Redistribution R5d/Interface/G_Graphics/GDirectX11Surface.h"

// Include DirectX11 for interface access
#include "Declarations.h"
#include "Mesh.h"
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
	D3D_DRIVER_TYPE			myDriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL		myFeatureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11RenderTargetView* myRenderTargetView = nullptr;
	ID3D11VertexShader*		myVertexShader = nullptr;
	ID3D11PixelShader*		myPixelShader = nullptr;
	ID3D11InputLayout*		myVertexLayout = nullptr;
	ID3D11Buffer*			myVertexBuffer = nullptr;
	ID3D11Buffer*			myIndexBuffer = nullptr;
	ID3D11Buffer*			myConstantBuffer = nullptr;
	XMMATRIX				worldMatrix;
	XMMATRIX				viewMatrix;
	XMMATRIX				projectionMatrix;

	Mesh charizard;
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

			myDevice->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), nullptr, &myVertexShader);
			myDevice->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), nullptr, &myPixelShader);

			// Define the input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE(layout);

			// Create the input layout
			myDevice->CreateInputLayout(layout, numElements, Trivial_VS, sizeof(Trivial_VS), &myVertexLayout);

			myContext->IASetInputLayout(myVertexLayout);
#pragma region FBX

			//// Change the following filename to a suitable filename value.
			//const char* lFilename = "\\Assets\\Charizard.fbx";

			//// Initialize the SDK manager. This object handles memory management.
			//FbxManager* lSdkManager = FbxManager::Create();
			//// Create the IO settings object.
			//FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
			//lSdkManager->SetIOSettings(ios);

			//// Create an importer using the SDK manager.
			//FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

			//// Use the first argument as the filename for the importer.
			//if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
			//	//printf("Call to FbxImporter::Initialize() failed.\n");
			//	//printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
			//	exit(-1);
			//}

			//// Create a new scene so that it can be populated by the imported file.
			//FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

			//// Import the contents of the file into the scene.
			//lImporter->Import(lScene);

			//// The file is imported, so get rid of the importer.
			//lImporter->Destroy();

			//ProcessFbxMesh(lScene->GetRootNode());
#pragma endregion

			charizard = Mesh("Charizard.fbx");

			D3D11_BUFFER_DESC bd = {};
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(SimpleVertex) *charizard.GetNumberOfVertices();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA InitData = {};
			InitData.pSysMem = charizard.GetVertices();
			myDevice->CreateBuffer(&bd, &InitData, &myVertexBuffer);

			// Set vertex buffer
			UINT stride = sizeof(SimpleVertex);
			UINT offset = 0;
			myContext->IASetVertexBuffers(0, 1, &myVertexBuffer, &stride, &offset);

			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(int) * charizard.GetNumberOfIndices();        // 36 vertices needed for 12 triangles in a triangle list
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			InitData.pSysMem = charizard.GetIndices();
			myDevice->CreateBuffer(&bd, &InitData, &myIndexBuffer);

			// Set index buffer
			myContext->IASetIndexBuffer(myIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

			// Set primitive topology
			myContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			// Create the constant buffer
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(ConstantBuffer);
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			myDevice->CreateBuffer(&bd, nullptr, &myConstantBuffer);

			// Initialize the world matrix
			worldMatrix = XMMatrixIdentity();

			// Initialize the view matrix
			XMVECTOR Eye = XMVectorSet(0.0f, 15.0f, -20.0f, 0.0f);
			XMVECTOR At = XMVectorSet(0.0f, 15.0f, 0.0f, 0.0f);
			XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			viewMatrix = XMMatrixLookAtLH(Eye, At, Up);

			// Initialize the projection matrix
			projectionMatrix = XMMatrixPerspectiveFovLH(XM_PIDIV2, width / (FLOAT)height, 0.01f, 100.0f);
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
	//myRenderTargetView->Release();
	myVertexShader->Release();
	myPixelShader->Release();
	myVertexLayout->Release();
	myVertexBuffer->Release();
	myIndexBuffer->Release();
	myConstantBuffer->Release();

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
			const float d_green[] = { 0, 0.5f, 0, 1 };
			myContext->ClearRenderTargetView(myRenderTargetView, d_green);

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

			//Update variables
			ConstantBuffer cb;
			cb.mWorld = XMMatrixTranspose(worldMatrix);
			cb.mView = XMMatrixTranspose(viewMatrix);
			cb.mProjection = XMMatrixTranspose(projectionMatrix);
			myContext->UpdateSubresource(myConstantBuffer, 0, nullptr, &cb, 0, 0);

			//
			// Renders a triangle
			//
			myContext->VSSetShader(myVertexShader, nullptr, 0);
			myContext->VSSetConstantBuffers(0, 1, &myConstantBuffer);
			myContext->PSSetShader(myPixelShader, nullptr, 0);
			myContext->DrawIndexed(charizard.GetNumberOfIndices(), 0, 0);        // 36 vertices needed for 12 triangles in a triangle list

													 // Present Backbuffer using Swapchain object
													 // Framerate is currently unlocked, we suggest "MSI Afterburner" to track your current FPS and memory usage.
			mySwapChain->Present(0, 0); // set first argument to 1 to enable vertical refresh sync with display

										// Free any temp DX handles aquired this frame
			myRenderTargetView->Release();
		}
	}
}

//void ProcessFbxMesh(FbxNode* Node)
//{
//	// set up output console
//	AllocConsole();
//	freopen("CONOUT$", "w", stdout);
//	freopen("CONOUT$", "w", stderr);
//
//	//FBX Mesh stuff
//	int childrenCount = Node->GetChildCount();
//
//	//cout << "\nName:" << Node->GetName();
//
//	for (int i = 0; i < childrenCount; i++)
//	{
//		FbxNode *childNode = Node->GetChild(i);
//		FbxMesh *mesh = childNode->GetMesh();
//
//		if (mesh != NULL)
//		{
//			//cout << "\nMesh:" << childNode->GetName();
//
//			// Get index count from mesh
//			numVertices = mesh->GetControlPointsCount();
//			//cout << "\nVertex Count:" << numVertices;
//
//			// Create SimpleVertex array to size of this mesh
//			vertices = new SimpleVertex[numVertices];
//
//			//================= Process Vertices ===================
//			for (int j = 0; j < numVertices; j++)
//			{
//				FbxVector4 vert = mesh->GetControlPointAt(j);
//				vertices[j].Pos.x = vert.mData[0] * scale;
//				vertices[j].Pos.y = vert.mData[1] * scale;
//				vertices[j].Pos.z = vert.mData[2] * scale;
//				vertices[j].Color = RAND_COLOR;
//				//cout << "\n" << vert.mData[0] << " " << vert.mData[1] << " " << vert.mData[2];
//			}
//
//			numIndices = mesh->GetPolygonVertexCount();
//			//cout << "\nIndice Count:" << numIndices;
//
//			// No need to allocate int array, FBX does for us
//			indices = mesh->GetPolygonVertices();
//
//			//================= Process Indices ====================
//			for (int j = 0; j < numIndices; j++)
//			{
//				//cout << "\nIndice:" << indices[j];
//			}
//		}
//		// recurse on all children
//		ProcessFbxMesh(childNode);
//	}
//}