#include "D3DObject.h"



D3DObject::D3DObject()
{
}

D3DObject::D3DObject(string fileName, float scale, ID3D11Device* &myDevice, ID3D11DeviceContext* &myContext, ID3D11VertexShader* &vertexShader, ID3D11PixelShader* &pixelShader, ID3D11Buffer* &perObjectBuffer)
{
	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_PerObjectBuffer = perObjectBuffer;
	m_Mesh = Mesh(fileName, scale, m_Device, m_TextureRV);

	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA InitData = {};
	stride[0] = sizeof(SimpleVertex);
	offset[0] = 0;

	//Desc Vertex Buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * m_Mesh.GetNumberOfVertices();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//Subresource
	InitData.pSysMem = m_Mesh.GetVertices();

	//Create Vertex Buffer
	m_Device->CreateBuffer(&bd, &InitData, &m_VertexBuffer);

	//Desc Index Buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(int) * m_Mesh.GetNumberOfIndices();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//Subresource
	InitData.pSysMem = m_Mesh.GetIndices();

	//Create Index Buffer
	m_Device->CreateBuffer(&bd, &InitData, &m_IndexBuffer);
}


void D3DObject::Render()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::SetPosition(XMVECTOR position, ConstantBuffer &constantBuffer)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position));
	m_Context->UpdateSubresource(m_PerObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}


D3DObject::~D3DObject()
{
	m_Mesh.LateDestructor();
}
