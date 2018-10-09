#pragma once
#include "Mesh.h"
class D3DObject
{
	bool makeRotating = false;
	Mesh m_Mesh;
	CComPtr<ID3D11Device> m_Device = nullptr;
	CComPtr < ID3D11DeviceContext> m_Context = nullptr;
	CComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;
	CComPtr < ID3D11Buffer> m_IndexBuffer = nullptr;
	CComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
	CComPtr < ID3D11PixelShader> m_PixelShader = nullptr;
	CComPtr<ID3D11GeometryShader> m_GeometryShader = nullptr;
	CComPtr < ID3D11ShaderResourceView> m_TextureRV = nullptr;
	CComPtr < ID3D11Buffer> m_PerObjectBuffer = nullptr;

	XMVECTOR m_Position = { 0,0,0,0 };
	UINT stride[1];
	UINT offset[1];

public:
	D3DObject();
	D3DObject(string fileName, float scale, CComPtr < ID3D11Device >& myDevice, CComPtr < ID3D11DeviceContext >& myContext,
		CComPtr < ID3D11VertexShader >& vertexShader, CComPtr < ID3D11PixelShader>& pixelShader,
		CComPtr < ID3D11GeometryShader> &geoShader, CComPtr < ID3D11Buffer> &perObjectBuffer);
	void RenderIndexed();
	void RenderIndexed(D3D_PRIMITIVE_TOPOLOGY topology);
	void RenderIndexedWithDynamicSRV(CComPtr < ID3D11ShaderResourceView> &textureRV);
	void RenderInstanced(int numberOfInstances, CComPtr < ID3D11Buffer> &perInstanceBuffer);
	void RenderIndexedMulitexture(CComPtr < ID3D11ShaderResourceView > textureRVs[]);
	void UpdateVS(CComPtr < ID3D11VertexShader > vertexShader);
	void UpdatePS(CComPtr < ID3D11PixelShader> pixelShader);
	void UpdateGS(CComPtr < ID3D11GeometryShader > geoShader);
	void UpdateTexture(string textureName);
	//do not use this unless you know what you are doing
	void UpdateTexture(CComPtr < ID3D11ShaderResourceView> &textureRV);
	void SetPosition(XMVECTOR position, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer);
	void SetPosition(XMMATRIX position, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer);
	void SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr < ID3D11Buffer >& perObjectBuffer, float factor);
	void SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr < ID3D11Buffer >& perObjectBuffer, float factorY, float factorX);
	void SetRotatingPosition(XMVECTOR position, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer, float rotationMatrixFactor);
	void SetRotatingPosition(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr < ID3D11Buffer >& perObjectBuffer, float rotationMatrixYFactor, float rotationMatrixXFactor);
	ID3D11PixelShader* GetPixelShader() { return m_PixelShader; }
	~D3DObject();

};

