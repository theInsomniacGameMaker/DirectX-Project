#pragma once
#include "Declarations.h"
#include "test pyramid.h"
#include "D3DObject.h"
SimpleVertex sphereVertices[360*17];
MeshStruct hFilePyramid;

float NormalLerp(int start, int end, float ratio)
{
	return ((end - start)*ratio) + start;
}

float DegToRad(float deg)
{
	return deg * (XM_PI / 180.0f);
}

bool wayToSort(D3DObject* &i, D3DObject* &j) { return i->m_DistanceFromCamera > j->m_DistanceFromCamera; }

void CreateSpiral()
{
	int p = 0;
	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 360; j++)
		{
			sphereVertices[p].Pos.x = (float)sin(DegToRad((float)j))*2.0f;
			sphereVertices[p].Pos.y = (float)cos(DegToRad((float)j))*2.0f;

			float n = i * 360.0f + j;
			float d = 360.0f * 17;
			 
			float ratio = n / d;
			sphereVertices[p].Pos.z = NormalLerp(1, 10, ratio);
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

void LoadFromHeader()
{
	hFilePyramid.numVertices = 1674;
	hFilePyramid.vertices = new SimpleVertex[1674];
	for (int i = 0; i < 1674; i += 3)
	{
		hFilePyramid.vertices[i] = ConvertFromHeaderToSimpleVertex(test_pyramid_data[test_pyramid_indicies[i]], 1);
		hFilePyramid.vertices[i + 1] = ConvertFromHeaderToSimpleVertex(test_pyramid_data[test_pyramid_indicies[i + 1]], 1);
		hFilePyramid.vertices[i + 2] = ConvertFromHeaderToSimpleVertex(test_pyramid_data[test_pyramid_indicies[i + 2]], 1);
	}
}



