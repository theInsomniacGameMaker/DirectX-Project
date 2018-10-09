#include "D3DObject.h"



D3DObject::D3DObject()
{
}

D3DObject::D3DObject(string fileName, float scale, ID3D11Device* &myDevice, ID3D11DeviceContext* &myContext, 
	ID3D11VertexShader* &vertexShader, ID3D11PixelShader* &pixelShader, 
	ID3D11GeometryShader* &geoShader, ID3D11Buffer* &perObjectBuffer)
{
	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_GeometryShader = geoShader;
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


void D3DObject::RenderIndexed()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderIndexed(D3D_PRIMITIVE_TOPOLOGY topology, ID3D11DeviceContext* &context)
{
	context->IASetPrimitiveTopology(topology);
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3DObject::RenderIndexedWithDynamicSRV(ID3D11ShaderResourceView *& textureRV)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &textureRV);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderInstanced(int numberOfInstances, ID3D11Buffer* &perInstanceBuffer)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->VSSetConstantBuffers(1, 1, &perInstanceBuffer);
	m_Context->DrawIndexedInstanced(m_Mesh.GetNumberOfIndices(), 10, 0, 0, 0);
}

void D3DObject::RenderIndexedMulitexture(ID3D11ShaderResourceView* textureRVs[])
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	//harcoding number of SRVs to 2
	m_Context->PSSetShaderResources(0, 2, textureRVs);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::UpdateVS(ID3D11VertexShader * vertexShader)
{
	m_VertexShader = vertexShader;
}

void D3DObject::UpdatePS(ID3D11PixelShader * pixelShader)
{
	m_PixelShader = pixelShader;
}

void D3DObject::UpdateGS(ID3D11GeometryShader * geoShader)
{
	m_GeometryShader = geoShader;
}

void D3DObject::UpdateTexture(string textureName)
{
	textureName = "Assets\\" + textureName + ".dds";
	std::wstring widestr = std::wstring(textureName.begin(), textureName.end());
	const wchar_t* widecstr = widestr.c_str();
	HRESULT hr = CreateDDSTextureFromFile(m_Device, widecstr, nullptr, &m_TextureRV);
}

void D3DObject::UpdateTexture(ID3D11ShaderResourceView *&textureRV)
{
	m_TextureRV = textureRV;
}

//Will set the position of the object
void D3DObject::SetPosition(XMVECTOR position, ConstantBuffer &constantBuffer, ID3D11Buffer* &perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}

void D3DObject::SetPosition(XMMATRIX position, ConstantBuffer & constantBuffer, ID3D11Buffer *& perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixTranspose((position));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}

void D3DObject::SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, ID3D11Buffer *& perObjectBuffer, float factor)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationY(factor)*(XMMatrixTranslationFromVector(position)));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}

void D3DObject::SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, ID3D11Buffer *& perObjectBuffer, float factorY, float factorX)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationY(factorY)*XMMatrixRotationX(factorX)*(XMMatrixTranslationFromVector(position)));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}

void D3DObject::SetRotatingPosition(XMVECTOR position, ConstantBuffer &constantBuffer, ID3D11Buffer* &perObjectBuffer, float rotationMatrixFactor)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position)*XMMatrixRotationY(rotationMatrixFactor));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}

void D3DObject::SetRotatingPosition(XMVECTOR position, ConstantBuffer &constantBuffer, ID3D11Buffer* &perObjectBuffer, float rotationMatrixYFactor, float rotationMatrixXFactor)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position)*XMMatrixRotationY(rotationMatrixYFactor)*XMMatrixRotationX(rotationMatrixXFactor));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
}

D3DObject::~D3DObject()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
	if (m_TextureRV)
	{
		m_TextureRV->Release();
		m_TextureRV = 0;
	}
	
	m_Mesh.LateDestructor();
}
