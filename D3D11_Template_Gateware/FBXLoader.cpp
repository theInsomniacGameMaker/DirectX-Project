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
			meshToMutate.indices[i] = (int)uniqueVertices.size() - 1;
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
		meshToMutate.vertices[i].Tex.x = uniqueVertices[i].Tex.x;
		meshToMutate.vertices[i].Tex.y = uniqueVertices[i].Tex.y;
	}

	// print out some stats
	/*cout << "\nindex count BEFORE/AFTER compaction " << numIndices;
	cout << "\nvertex count ORIGINAL (FBX source): " << numVertices;
	cout << "\nvertex count AFTER expansion: " << numIndices;
	cout << "\nvertex count AFTER compaction: " << uniqueVertices.size();
	cout << "\nSize reduction: " << ((numVertices - uniqueVertices.size()) / (float)numVertices)*100.00f << "%";
	cout << "\nor " << (uniqueVertices.size() / (float)numVertices) << " of the expanded size";*/
}

void ProcessFbxMesh(FbxNode* Node, MeshStruct &meshToMutate, ID3D11Device *&myDevice, ID3D11ShaderResourceView*& myTextureRV)
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
				meshToMutate.vertices[j].Pos.x = (float)vert.mData[0] * meshToMutate.scale;
				meshToMutate.vertices[j].Pos.y = (float)vert.mData[1] * meshToMutate.scale;
				meshToMutate.vertices[j].Pos.z = (float)vert.mData[2] * meshToMutate.scale;
				// Generate random normal
				// vertices[j].Normal = RAND_NORMAL;
				meshToMutate.vertices[j].TPos = XMFLOAT3(0, 0, 0);
			}


			// Get the Normals array from the mesh
			FbxArray<FbxVector4> normalsVec;
			mesh->GetPolygonVertexNormals(normalsVec);
			cout << "\nNormalVec Count:" << normalsVec.Size();

			// Declare a new array for the second vertex array
			// Note the size is numIndices not numVertices
			SimpleVertex *vertices2 = new SimpleVertex[meshToMutate.numIndices];

			// align (expand) vertex array and set the normals
			for (int j = 0; j < meshToMutate.numIndices; j++)
			{
				vertices2[j] = meshToMutate.vertices[meshToMutate.indices[j]];
				vertices2[j].Normal.x = (float)normalsVec[j].mData[0];
				vertices2[j].Normal.y = (float) normalsVec[j].mData[1];
				vertices2[j].Normal.z = (float)normalsVec[j].mData[2];
			}

			int materialCount = childNode->GetSrcObjectCount<FbxSurfaceMaterial>();

			for (int index = 0; index < materialCount; index++)
			{
				FbxSurfaceMaterial* material = (FbxSurfaceMaterial*)childNode->GetSrcObject<FbxSurfaceMaterial>(index);

				if (material != NULL)
				{
					cout << "\nmaterial: " << material->GetName() << std::endl;
					// This only gets the material of type sDiffuse, you probably need to traverse all Standard Material Property by its name to get all possible textures.
					FbxProperty prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

					// Check if it's layeredtextures
					int layeredTextureCount = prop.GetSrcObjectCount<FbxLayeredTexture>();

					if (layeredTextureCount > 0)
					{
						for (int j = 0; j < layeredTextureCount; j++)
						{
							FbxLayeredTexture* layered_texture = FbxCast<FbxLayeredTexture>(prop.GetSrcObject<FbxLayeredTexture>(j));
							int lcount = layered_texture->GetSrcObjectCount<FbxTexture>();

							for (int k = 0; k < lcount; k++)
							{
								FbxFileTexture* texture = FbxCast<FbxFileTexture>(layered_texture->GetSrcObject<FbxTexture>(k));
								// Then, you can get all the properties of the texture, include its name
								const char* textureName = texture->GetFileName();
								cout << textureName;
							}
						}
					}
					else
					{
						// Directly get textures
						int textureCount = prop.GetSrcObjectCount<FbxTexture>();

						for (int j = 0; j < textureCount; j++)
						{
							FbxFileTexture* texture = FbxCast<FbxFileTexture>(prop.GetSrcObject<FbxTexture>(j));
							// Then, you can get all the properties of the texture, include its name
							const char* textureName = texture->GetFileName();
							cout << textureName;

							string appendedTextureName = (texture->GetFileName());
							int pos = (int)appendedTextureName.find_last_of('\\');
							if (pos != -1)
							{
								appendedTextureName = appendedTextureName.substr(pos + 1, appendedTextureName.length());
								appendedTextureName = "Assets\\" + appendedTextureName.substr(0, appendedTextureName.length() - 3) + "dds";
							}
							else
							{
								pos = (int)appendedTextureName.find_last_of('/');
								appendedTextureName = appendedTextureName.substr(pos + 1, appendedTextureName.length());
								appendedTextureName = "Assets\\" + appendedTextureName.substr(0, appendedTextureName.length() - 3) + "dds";
							}

							std::wstring widestr = std::wstring(appendedTextureName.begin(), appendedTextureName.end());
							const wchar_t* widecstr = widestr.c_str();

							if (myTextureRV == nullptr)
							{
								HRESULT hr = CreateDDSTextureFromFile(myDevice, widecstr, nullptr, &myTextureRV);
							}

							FbxProperty p = texture->RootProperty.Find("Filename");
							cout << p.Get<FbxString>() << std::endl;
						}
					}
				}
			}


			//get all UV set names
			FbxStringList lUVSetNameList;
			mesh->GetUVSetNames(lUVSetNameList);

			//iterating over all uv sets
			for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
			{
				//get lUVSetIndex-th uv set
				const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
				const FbxGeometryElementUV* lUVElement = mesh->GetElementUV(lUVSetName);

				if (!lUVElement)
					continue;

				// only support mapping mode eByPolygonVertex and eByControlPoint
				if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
					lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
					return;

				//index array, where holds the index referenced to the uv data
				const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
				const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

				//iterating through the data by polygon
				const int lPolyCount = mesh->GetPolygonCount();


				int lPolyIndexCounter = 0;
				for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
				{
					// build the max index array that we need to pass into MakePoly
					const int lPolySize = mesh->GetPolygonSize(lPolyIndex);
					for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
					{
						if (lPolyIndexCounter < lIndexCount)
						{
							FbxVector2 lUVValue;

							//the UV index depends on the reference mode
							int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

							lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

							//User TODO:
							//Print out the value of UV(lUVValue) or log it to a file

							vertices2[lPolyIndexCounter].Tex.x = (float)lUVValue.mData[0];
							vertices2[lPolyIndexCounter].Tex.y = (float)lUVValue.mData[1];
							lPolyIndexCounter++;
						}
					}
				}
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

			if (false)
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
		ProcessFbxMesh(childNode, meshToMutate, myDevice, myTextureRV);
	}
	
}

