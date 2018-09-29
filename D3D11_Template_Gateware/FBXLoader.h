#pragma once
#include "Declarations.h"
#include "DDSTextureLoader.h"
void ProcessFbxMesh(FbxNode* Node, MeshStruct &meshToMutate, ID3D11Device *&myDevice, ID3D11ShaderResourceView*&   myTextureRV);