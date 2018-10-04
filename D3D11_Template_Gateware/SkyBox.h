#pragma once
#include "Declarations.h"
class SkyBox
{
public:
	vector<SimpleVertex> vertices;
	vector<int> indices;

	MeshStruct skyMesh;
	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;
	SkyBox();
	SkyBox(int LatLines, int LongLines);
	~SkyBox();
};

