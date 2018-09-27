#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <fbxsdk.h>
#include <iostream>
using namespace DirectX;
using namespace std;


#include "Trivial_VS.csh"
#include "Trivial_PS.csh"

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};


struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
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