#include "FBXLoader.h"


bool CheckEpsilonEquality(float x, float y)
{

	if (fabs(x - y) <= numeric_limits<float>::epsilon())
	{
		return true;
	}
	return false;
}

void Compactify(MeshStruct &meshToMutate)
{
	vector <SimpleVertex> uniqueVertices;
	//uniqueVertices.resize(meshToMutate.numVertices);
	for (int i = 0; i < meshToMutate.numIndices; i++)
	{
		bool foundSomething = false;

		for (int j = 0; j < uniqueVertices.size(); j++)
		{
			if (CheckEpsilonEquality(meshToMutate.vertices[i].Pos.x, uniqueVertices[j].Pos.x) &&
				CheckEpsilonEquality(meshToMutate.vertices[i].Pos.y, uniqueVertices[j].Pos.y) &&
				CheckEpsilonEquality(meshToMutate.vertices[i].Pos.z, uniqueVertices[j].Pos.z) &&
				CheckEpsilonEquality(meshToMutate.vertices[i].Normal.x, uniqueVertices[j].Normal.x) &&
				CheckEpsilonEquality(meshToMutate.vertices[i].Normal.y, uniqueVertices[j].Normal.y) &&
				CheckEpsilonEquality(meshToMutate.vertices[i].Normal.z, uniqueVertices[j].Normal.z))
			{
				meshToMutate.indices[i] = j;
				foundSomething = true;
				break;
			}

		}
		if (!foundSomething)
		{
			uniqueVertices.push_back(meshToMutate.vertices[i]);
			meshToMutate.indices[i] = uniqueVertices.size() - 1;
		}
	}

	delete meshToMutate.vertices;

	meshToMutate.vertices = new SimpleVertex[uniqueVertices.size()];

	for (int i = 0; i < uniqueVertices.size(); i++)
	{
		meshToMutate.vertices[i].Pos.x = uniqueVertices[i].Pos.x;
		meshToMutate.vertices[i].Pos.y = uniqueVertices[i].Pos.y;
		meshToMutate.vertices[i].Pos.z = uniqueVertices[i].Pos.z;
		meshToMutate.vertices[i].Normal.x = uniqueVertices[i].Normal.x;
		meshToMutate.vertices[i].Normal.y = uniqueVertices[i].Normal.y;
		meshToMutate.vertices[i].Normal.z = uniqueVertices[i].Normal.z;
	}

	// print out some stats
	/*cout << "\nindex count BEFORE/AFTER compaction " << meshToMutate.numIndices;
	cout << "\nvertex count ORIGINAL (FBX source): " << meshToMutate.numVertices;
	cout << "\nvertex count AFTER expansion: " << numIndices;
	cout << "\nvertex count AFTER compaction: " << uniqueVertices.size();
	cout << "\nSize reduction: " << ((numVertices - uniqueVertices.size()) / (float)numVertices)*100.00f << "%";
	cout << "\nor " << (uniqueVertices.size() / (float)numVertices) << " of the expanded size";*/
}


void ProcessFbxMesh(FbxNode* Node, MeshStruct &meshToMutate)
{// set up output console
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	//FBX Mesh stuff
	int childrenCount = Node->GetChildCount();

	cout << "\nName:" << Node->GetName();

	for (int i = 0; i < childrenCount; i++)
	{
		FbxNode *childNode = Node->GetChild(i);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			cout << "\nMesh:" << childNode->GetName();

			// Get index count from mesh
			meshToMutate.numIndices = mesh->GetPolygonVertexCount();
			cout << "\nIndice Count:" << meshToMutate.numIndices;

			// No need to allocate int array, FBX does for us
			meshToMutate.indices = mesh->GetPolygonVertices();

			// Get vertex count from mesh
			meshToMutate.numVertices = mesh->GetControlPointsCount();
			cout << "\nVertex Count:" << meshToMutate.numVertices;

			// Create SimpleVertex array to size of this mesh
			meshToMutate.vertices = new SimpleVertex[meshToMutate.numVertices];

			//================= Process Vertices ===================
			for (int j = 0; j < meshToMutate.numVertices; j++)
			{
				FbxVector4 vert = mesh->GetControlPointAt(j);
				meshToMutate.vertices[j].Pos.x = (float)vert.mData[0] / meshToMutate.scale;
				meshToMutate.vertices[j].Pos.y = (float)vert.mData[1] / meshToMutate.scale;
				meshToMutate.vertices[j].Pos.z = (float)vert.mData[2] / meshToMutate.scale;
				// Generate random normal
				//vertices[j].Normal = RAND_NORMAL;
			}

			// Get the Normals array from the mesh
			FbxArray<FbxVector4> normalsVec;
			mesh->GetPolygonVertexNormals(normalsVec);
			cout << "\nNormalVec Count:" << normalsVec.Size();

			// Declare a new array for the second vertex array
			// Note the size is meshToMutate.numIndices not numVertices
			SimpleVertex *vertices2 = new SimpleVertex[meshToMutate.numIndices];

			// align (expand) vertex array and set the normals
			for (int j = 0; j < meshToMutate.numIndices; j++)
			{
				vertices2[j] = meshToMutate.vertices[meshToMutate.indices[j]];
				vertices2[j].Normal.x = normalsVec[j].mData[0];
				vertices2[j].Normal.y = normalsVec[j].mData[1];
				vertices2[j].Normal.z = normalsVec[j].mData[2];
			}

			// vertices is an "out" var so make sure it points to the new array
			// and clean up first array
			delete meshToMutate.vertices;
			meshToMutate.vertices = vertices2;

			// make new indices to match the new vertex(2) array
			delete meshToMutate.indices;
			meshToMutate.indices = new int[meshToMutate.numIndices];
			for (int j = 0; j < meshToMutate.numIndices; j++)
			{
				meshToMutate.indices[j] = j;
			}

			if (true)
			{
				Compactify(meshToMutate);
			}
			else
			{
				// numVertices is an "out" var so set to new size
				// this is used in the DrawIndexed functions to set the
				// the right number of triangles
				meshToMutate.numVertices = meshToMutate.numIndices;
			}
		}
		ProcessFbxMesh(childNode,meshToMutate);
	}
	
}