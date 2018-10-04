#pragma once
#include "Declarations.h"
class SkyBox
{
public:
	MeshStruct skyMesh;
	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;
	SkyBox(int LatLines, int LongLines);
	~SkyBox();
};

