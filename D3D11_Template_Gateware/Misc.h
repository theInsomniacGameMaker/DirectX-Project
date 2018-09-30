#pragma once
#include "Declarations.h"

int numSphereVertices, sphereFaces, numSphereIndices;
vector<SimpleVertex> sphereVertices;
vector<int> indices;
void CreateSphere(int LatLines, int LongLines, ID3D11Device *&myDevice, ID3D11Buffer*&sphereVertexBuffer, ID3D11Buffer*&sphereIndexBuffer)
{
	numSphereVertices = ((LatLines - 2) * LongLines) + 2;
	sphereFaces = ((LatLines - 3)*(LongLines) * 2) + (LongLines * 2);

	indices.resize(sphereFaces * 3);
	sphereVertices.resize(numSphereVertices);

	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;

	//std::vector<SimpleVertex> vertices(sphereVertices);

	XMVECTOR currVertPos = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	sphereVertices[0].Pos.x = 0.0f;
	sphereVertices[0].Pos.y = 0.0f;
	sphereVertices[0].Pos.z = 1.0f;

	XMMATRIX RotationX, RotationY;
	for (int i = 0; i < LatLines - 2; ++i)
	{
		spherePitch = (i + 1) * (3.14 / (LatLines - 1));
		RotationX = XMMatrixRotationX(spherePitch);
		for (int j = 0; j < LongLines; ++j)
		{
			sphereYaw = j * (6.28 / (LongLines));
			RotationY = XMMatrixRotationZ(sphereYaw);
			currVertPos = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (RotationX * RotationY));
			currVertPos = XMVector3Normalize(currVertPos);
			sphereVertices[i*LongLines + j + 1].Pos.x = XMVectorGetX(currVertPos);
			sphereVertices[i*LongLines + j + 1].Pos.y = XMVectorGetY(currVertPos);
			sphereVertices[i*LongLines + j + 1].Pos.z = XMVectorGetZ(currVertPos);
			sphereVertices[i*LongLines + j + 1].Normal.x = XMVectorGetX(currVertPos);
			sphereVertices[i*LongLines + j + 1].Normal.y = XMVectorGetY(currVertPos);
			sphereVertices[i*LongLines + j + 1].Normal.z = XMVectorGetZ(currVertPos);
		}
	}

	sphereVertices[numSphereVertices - 1].Pos.x = 0.0f;
	sphereVertices[numSphereVertices - 1].Pos.y = 0.0f;
	sphereVertices[numSphereVertices - 1].Pos.z = -1.0f;

	int k = 0;
	for (int l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = 0;
		indices[k + 1] = l + 1;
		indices[k + 2] = l + 2;
		k += 3;
	}

	indices[k] = 0;
	indices[k + 1] = LongLines;
	indices[k + 2] = 1;
	k += 3;

	for (int i = 0; i < LatLines - 3; ++i)
	{
		for (int j = 0; j < LongLines - 1; ++j)
		{
			indices[k] = i * LongLines + j + 1;
			indices[k + 1] = i * LongLines + j + 2;
			indices[k + 2] = (i + 1)*LongLines + j + 1;

			indices[k + 3] = (i + 1)*LongLines + j + 1;
			indices[k + 4] = i * LongLines + j + 2;
			indices[k + 5] = (i + 1)*LongLines + j + 2;

			k += 6; // next quad
		}

		indices[k] = (i*LongLines) + LongLines;
		indices[k + 1] = (i*LongLines) + 1;
		indices[k + 2] = ((i + 1)*LongLines) + LongLines;

		indices[k + 3] = ((i + 1)*LongLines) + LongLines;
		indices[k + 4] = (i*LongLines) + 1;
		indices[k + 5] = ((i + 1)*LongLines) + 1;

		k += 6;
	}

	for (int l = 0; l < LongLines - 1; ++l)
	{
		indices[k] = numSphereVertices - 1;
		indices[k + 1] = (numSphereVertices - 1) - (l + 1);
		indices[k + 2] = (numSphereVertices - 1) - (l + 2);
		k += 3;
	}

	indices[k] = numSphereVertices - 1;
	indices[k + 1] = (numSphereVertices - 1) - LongLines;
	indices[k + 2] = numSphereVertices - 2;

	numSphereIndices = indices.size();
}

