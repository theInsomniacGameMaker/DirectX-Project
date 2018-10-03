#pragma once
#include "Declarations.h"
#include "test pyramid.h"

SimpleVertex sphereVertices[360*17];
MeshStruct returnStruct;

float NormalLerp(int start, int end, float ratio)
{
	return ((end - start)*ratio) + start;
}

float DegToRad(float deg)
{
	return deg * (XM_PI / 180.0f);
}

void CreateSpiral()
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

SimpleVertex ConvertFromHeaderToSimpleVertex(_OBJ_VERT_ obj, float scale)
{
	SimpleVertex a;
	a.Pos.x = obj.pos[0] *scale;
	a.Pos.y = obj.pos[1] *scale;
	a.Pos.z = obj.pos[2] *scale;
	
	a.Normal.x = obj.nrm[0];
	a.Normal.y = obj.nrm[1];
	a.Normal.z = obj.nrm[2];

	return a;
}

void  LoadFromHeader()
{
	returnStruct.numVertices = 1674;
	returnStruct.vertices = new SimpleVertex[1674];
	for (int i = 0; i < 1674; i+=3)
	{
		returnStruct.vertices[i]= ConvertFromHeaderToSimpleVertex(test_pyramid_data[test_pyramid_indicies[i]],1	);
		returnStruct.vertices[i+1]= ConvertFromHeaderToSimpleVertex(test_pyramid_data[test_pyramid_indicies[i + 1]],1);
		returnStruct.vertices[i + 2] = ConvertFromHeaderToSimpleVertex(test_pyramid_data[test_pyramid_indicies[i + 2]],1);
	}

	/*returnStruct.numIndices = 1674;
	returnStruct.indices = new int[returnStruct.numIndices];
	for (int i = 0; i < 1674; i++)
	{
		returnStruct.indices[i] = test_pyramid_indicies[i];
	}*/

}

