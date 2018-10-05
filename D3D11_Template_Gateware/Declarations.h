#pragma once
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <atlbase.h>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <dinput.h>
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#include <time.h>
using namespace DirectX;
using namespace std;


#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "VS_UVModifier.csh"
#include "SolidPS.csh"
#include "PS_SkyBox.csh"
#include "VS_SkyBox.csh"
#include "VS_Instance.csh"

#define CHARIZARD_MESH 1
#define BOX_MESH 1
#define SPACESHIP 0
#define PROCEDURAL_SPIRAL 1
#define WIREFRAME 0
#define DIRECTIONAL_LIGHT_ON 1
#define NON_MOUSE_CAMERA_MOVEMENT 1
#define DEBUGGER 0

static const XMVECTOR UP = { 0,1,0,0 };
static const XMVECTOR RIGHT = { 1,0,0,0 };
static const XMVECTOR FORWARD = { 0,0,1,0 };

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT4 Tangent;
};

struct Light
{
	XMFLOAT4 Position;
	XMFLOAT4 Color;
	XMFLOAT4 Direction;
	XMFLOAT4 Range;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 time;
	XMFLOAT4 vOutputColor;
};

struct InstanceConstantBuffer
{
	XMMATRIX worldArray[10];
};

struct LightConstantBuffer
{
	Light lights[3];
};

// Arrays and counts for
// FBX to DirectX data transfer

struct MeshStruct
{
	SimpleVertex *vertices;
	int numVertices = 0;
	int *indices;
	int numIndices = 0;
	float scale = 1.0f;
};

// funtime random color
#define RAND_COLOR XMFLOAT4(rand()/float(RAND_MAX),rand()/float(RAND_MAX),rand()/float(RAND_MAX),1.0f)
