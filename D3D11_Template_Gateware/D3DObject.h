#pragma once
#include "Mesh.h"
class D3DObject
{
	Mesh m_Mesh;
	ID3D11Device* m_Device = nullptr;
	ID3D11DeviceContext* m_Context= nullptr;
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11Buffer* m_IndexBuffer = nullptr;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11ShaderResourceView* m_TextureRV;
	ID3D11Buffer* m_PerObjectBuffer = nullptr;

	XMVECTOR m_Position = {0,0,0,0};
	UINT stride[1];
	UINT offset[1];

public:
	D3DObject(string fileName, float scale, ID3D11Device* &myDevice, ID3D11DeviceContext* &myContext, ID3D11VertexShader* &vertexShader, ID3D11PixelShader* &pixelShader, ID3D11Buffer* &perObjectBuffer);
	D3DObject();
	void Render();
	void SetPosition(XMVECTOR position, ConstantBuffer &constantBuffer);
	~D3DObject();

};

