#pragma once
#include "FBXLoader.h"

class Mesh
{
	MeshStruct myMesh;

public:
	Mesh();
	Mesh(string fileName);
	~Mesh();
	SimpleVertex* GetVertices();
	int GetNumberOfVertices();
	int* GetIndices();
	int GetNumberOfIndices();
	void SetScale(float scale);

};

