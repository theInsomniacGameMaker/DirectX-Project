#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <fbxsdk.h>
#include <iostream>
#include <vector>
#include <time.h>
using namespace DirectX;
using namespace std;


#include "Trivial_VS.csh"
#include "Trivial_PS.csh"

#define CHARIZARD_MESH 0
#define BOX_MESH 1
#define PROCEDURAL_SPHERE 1
#define WIREFRAME 0
#define DIRECTIONAL_LIGHT_ON 0

static const XMVECTOR UP = { 0,1,0,0 };
static const XMVECTOR RIGHT = { 1,0,0,0 };
static const XMVECTOR FORWARD = { 0,0,1,0 };

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 Tex;
	XMFLOAT3 TPos;
};

struct PointLight
{
	XMFLOAT4 pos;
	float range;
	XMFLOAT3 att;
	XMFLOAT4 diffuse;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
	XMFLOAT4 vLightDir[2];
	XMFLOAT4 vLightColor[2];
	PointLight pointLight;
	float time;
	XMFLOAT4 vOutputColor;
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
