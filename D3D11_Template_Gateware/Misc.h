#pragma once
#include "Declarations.h"

int numSphereVertices, sphereFaces, numSphereIndices;
SimpleVertex sphereVertices[360*17];

float NormalLerp(int start, int end, float ratio)
{
	return ((end - start)*ratio) + start;
}

float DegToRad(float deg)
{
	return deg * (XM_PI / 180.0f);
}

void CreateGrid()
{
	int p = 0;
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 360; j++)
		{
			SimpleVertex s;
			sphereVertices[p].Pos.x = sin(DegToRad(j))*2;
			sphereVertices[p].Pos.y = cos(DegToRad(j))*2;

			float n = i * 360 + j;
			float d = 360 * 17;

			float ratio = n / d;
			sphereVertices[p].Pos.z = NormalLerp(1.0f, 10.0f, ratio);
			//sphereVertices[p].Pos = s;
			p++;
		}
	}
}

