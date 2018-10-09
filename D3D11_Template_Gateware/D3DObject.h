#pragma once
#include "Mesh.h"
class D3DObject
{
	bool makeRotating=false;
	Mesh m_Mesh;
	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_Context= nullptr;
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11GeometryShader* m_GeometryShader = nullptr;
	ID3D11ShaderResourceView* m_TextureRV = nullptr;
	ID3D11Buffer* m_PerObjectBuffer = nullptr;

	XMVECTOR m_Position = {0,0,0,0};
	UINT stride[1];
	UINT offset[1];

public:
	D3DObject();
	D3DObject(string fileName, float scale, ID3D11Device *& myDevice, ID3D11DeviceContext *& myContext, 
		ID3D11VertexShader *& vertexShader, ID3D11PixelShader *& pixelShader, 
		ID3D11GeometryShader* &geoShader, ID3D11Buffer* &perObjectBuffer);	
	void RenderIndexed();
	void RenderIndexed(D3D_PRIMITIVE_TOPOLOGY topology, ID3D11DeviceContext *& context);
	void RenderInstanced(int numberOfInstances, ID3D11Buffer* &perInstanceBuffer);
	void RenderIndexedMulitexture(ID3D11ShaderResourceView * textureRVs[]);
	void UpdateVS(ID3D11VertexShader *& vertexShader);
	void UpdatePS(ID3D11PixelShader*& pixelShader);
	void UpdateGS(ID3D11GeometryShader *& geoShader);
	void UpdateTexture(string textureName);
	void UpdateTexture(ID3D11ShaderResourceView* &textureRV);
	void SetPosition(XMVECTOR position, ConstantBuffer &constantBuffer, ID3D11Buffer* &perObjectBuffer);
	void SetPosition(XMMATRIX position, ConstantBuffer &constantBuffer, ID3D11Buffer* &perObjectBuffer);
	void SetRotatingPosition(XMVECTOR position, ConstantBuffer &constantBuffer, ID3D11Buffer* &perObjectBuffer, float rotationMatrixFactor);
	ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }
	~D3DObject();

};

