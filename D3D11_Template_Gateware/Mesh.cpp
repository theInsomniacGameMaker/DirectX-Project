#include "Mesh.h"


Mesh::Mesh()
{

}
Mesh::Mesh(string fileName, float scale)
{
	fileName = "\\Assets\\" + fileName;
	// Change the following filename to a suitable filename value.
	const char* lFilename =fileName.c_str();

	// Initialize the SDK manager. This object handles memory management.
	FbxManager* lSdkManager = FbxManager::Create();
	// Create the IO settings object.
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	// Create an importer using the SDK manager.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");

	// Use the first argument as the filename for the importer.
	if (!lImporter->Initialize(lFilename, -1, lSdkManager->GetIOSettings())) {
		exit(-1);
	}

	// Create a new scene so that it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	// Import the contents of the file into the scene.
	lImporter->Import(lScene);

	// The file is imported, so get rid of the importer.
	lImporter->Destroy();
	myMesh.scale = scale;
	ProcessFbxMesh(lScene->GetRootNode(), myMesh);
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
	
}
