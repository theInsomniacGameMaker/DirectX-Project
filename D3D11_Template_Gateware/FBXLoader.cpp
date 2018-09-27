#include "FBXLoader.h"

void ProcessFbxMesh(FbxNode* Node, MeshStruct &meshToMutate)
{
	// set up output console
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	//FBX Mesh stuff
	int childrenCount = Node->GetChildCount();

	//cout << "\nName:" << Node->GetName();

	for (int i = 0; i < childrenCount; i++)
	{
		FbxNode *childNode = Node->GetChild(i);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			//cout << "\nMesh:" << childNode->GetName();

			// Get index count from mesh
			 meshToMutate.numVertices = mesh->GetControlPointsCount();
			//cout << "\nVertex Count:" << numVertices;

			// Create SimpleVertex array to size of this mesh
			 meshToMutate.vertices = new SimpleVertex[meshToMutate.numVertices];

			//================= Process Vertices ===================
			for (int j = 0; j < meshToMutate.numVertices; j++)
			{
				FbxVector4 vert = mesh->GetControlPointAt(j);
				meshToMutate.vertices[j].Pos.x = vert.mData[0] * meshToMutate.scale;
				meshToMutate.vertices[j].Pos.y = vert.mData[1] * meshToMutate.scale;
				meshToMutate.vertices[j].Pos.z = vert.mData[2] * meshToMutate.scale;
				meshToMutate.vertices[j].Color = RAND_COLOR;
				//cout << "\n" << vert.mData[0] << " " << vert.mData[1] << " " << vert.mData[2];
			}

			meshToMutate.numIndices = mesh->GetPolygonVertexCount();
			//cout << "\nIndice Count:" << numIndices;

			// No need to allocate int array, FBX does for us
			meshToMutate.indices = mesh->GetPolygonVertices();

			//================= Process Indices ====================
			for (int j = 0; j < meshToMutate.numIndices; j++)
			{
				//cout << "\nIndice:" << indices[j];
			}
		}
		// recurse on all children
		ProcessFbxMesh(childNode,meshToMutate);
	}
}