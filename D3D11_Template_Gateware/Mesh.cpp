#include "Mesh.h"


Mesh::Mesh()
{

}
Mesh::Mesh(string fileName, float scale, ID3D11Device* &myDevice, ID3D11ShaderResourceView* &myTextureRV)
{
	fileName = "\\Assets\\" + fileName;
	// Change the following filename to a suitable filename value.
	const char* lFilename =fileName.c_str();

	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "");

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		exit(-1);
	}

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	/*FbxGeometryConverter converter(lSdkManager);
	if (!converter.Triangulate(lScene, true))return;
	if (!converter.SplitMeshesPerMaterial(lScene, true))return;*/

	myMesh.scale = scale;
	ProcessFbxMesh(lScene->GetRootNode(), myMesh, myDevice, myTextureRV);
	//vector<string> texFiles;
	//MeshStruct newMeshStruct;
	//ProcessFbxMesh2(lScene->GetRootNode(), myMesh, texFiles);
	int debug = 0;
}

SimpleVertex* Mesh::GetVertices()
{
	return myMesh.vertices;
}
int Mesh::GetNumberOfVertices()
{
	return myMesh.numVertices;
}
int* Mesh::GetIndices()
{
	return myMesh.indices;
}
int Mesh::GetNumberOfIndices()
{
	return myMesh.numIndices;
}

void Mesh::SetScale(float s)
{
	myMesh.scale = s;
}

Mesh::~Mesh()
{
	//delete[] myMesh.indices;
	//delete[] myMesh.vertices;
}

void Mesh::LateDestructor()
{
	delete[] myMesh.indices;
	delete[] myMesh.vertices;
}
