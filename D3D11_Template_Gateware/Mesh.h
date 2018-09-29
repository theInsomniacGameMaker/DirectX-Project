#pragma once
#include "FBXLoader.h"

class Mesh
{
	MeshStruct myMesh;

public:
	Mesh();
	Mesh(string fileName, float scale, ID3D11Device* &myDevice, ID3D11ShaderResourceView* &myTextureRV);
	~Mesh();
	SimpleVertex* GetVertices();
	int GetNumberOfVertices();
	int* GetIndices();
	int GetNumberOfIndices();
	void SetScale(float scale);

};

