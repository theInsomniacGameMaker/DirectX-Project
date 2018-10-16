#include "D3DObject.h"



D3DObject::D3DObject()
{
}

D3DObject::D3DObject(string fileName, float scale, CComPtr < ID3D11Device >& myDevice, 
	CComPtr < ID3D11DeviceContext >& myContext,CComPtr < ID3D11VertexShader >& vertexShader, 
	CComPtr < ID3D11PixelShader>& pixelShader,CComPtr < ID3D11GeometryShader> &geoShader, 
	CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_GeometryShader = geoShader;
	m_PerObjectBuffer = perObjectBuffer;
	m_Mesh = Mesh(fileName, scale, m_Device.p, m_TextureRV.p);

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

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	HRESULT hr = m_Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = m_Device->CreateRasterizerState(&cmdesc, &CWcullMode);
}

D3DObject::D3DObject(string fileName, float scale, CComPtr<ID3D11Device>& myDevice, 
	CComPtr<ID3D11DeviceContext>& myContext, CComPtr<ID3D11VertexShader>& vertexShader, 
	CComPtr<ID3D11PixelShader>& pixelShader, CComPtr<ID3D11GeometryShader>& geoShader, 
	CComPtr<ID3D11Buffer>& perObjectBuffer, string explicitTexture)
{
	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_GeometryShader = geoShader;
	m_PerObjectBuffer = perObjectBuffer;
	m_Mesh = Mesh(fileName, scale, m_Device.p, m_TextureRV.p);

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

	UpdateTexture(explicitTexture);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	HRESULT hr = m_Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = m_Device->CreateRasterizerState(&cmdesc, &CWcullMode);


}

D3DObject::D3DObject(string fileName, float scale, CComPtr<ID3D11Device>& myDevice, CComPtr<ID3D11DeviceContext>& myContext, 
	CComPtr<ID3D11VertexShader>& vertexShader, CComPtr<ID3D11PixelShader>& pixelShader, CComPtr<ID3D11GeometryShader>& geoShader, 
	CComPtr<ID3D11Buffer>& perObjectBuffer, string specialTexFileName, TEXTURE_TYPE textureType)
{

	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_GeometryShader = geoShader;
	m_PerObjectBuffer = perObjectBuffer;
	m_Mesh = Mesh(fileName, scale, m_Device.p, m_TextureRV.p);

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

	if (m_SpecialTexture)
	{
		m_SpecialTexture.Release();
		m_SpecialTexture = nullptr;
	}
	string specialTextureName = "Assets\\" + specialTextureName + ".dds";
	std::wstring widestr = std::wstring(specialTextureName.begin(), specialTextureName.end());
	const wchar_t* widecstr = widestr.c_str();
	HRESULT hr = CreateDDSTextureFromFile(m_Device, widecstr, nullptr, &m_SpecialTexture);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = m_Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = m_Device->CreateRasterizerState(&cmdesc, &CWcullMode);

}

D3DObject::D3DObject(string fileName, float scale, CComPtr<ID3D11Device>& myDevice,
	CComPtr<ID3D11DeviceContext>& myContext, CComPtr<ID3D11VertexShader>& vertexShader,
	CComPtr<ID3D11PixelShader>& pixelShader, CComPtr<ID3D11GeometryShader>& geoShader,
	CComPtr<ID3D11Buffer>& perObjectBuffer, string explicitTexture, string specialTexFileName, TEXTURE_TYPE textureType)
{
	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_GeometryShader = geoShader;
	m_PerObjectBuffer = perObjectBuffer;
	m_Mesh = Mesh(fileName, scale, m_Device.p, m_TextureRV.p);

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

	UpdateTexture(explicitTexture);


	if (m_SpecialTexture)
	{
		m_SpecialTexture.Release();
		m_SpecialTexture = nullptr;
	}
	string specialTextureName = "Assets\\" + specialTexFileName + ".dds";
	std::wstring widestr = std::wstring(specialTextureName.begin(), specialTextureName.end());
	const wchar_t* widecstr = widestr.c_str();
	HRESULT hr = CreateDDSTextureFromFile(m_Device, widecstr, nullptr, &m_SpecialTexture);

	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));

	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;

	cmdesc.FrontCounterClockwise = true;
	hr = m_Device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = m_Device->CreateRasterizerState(&cmdesc, &CWcullMode);


}


void D3DObject::RenderIndexed()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::ImmediatePositionRenderingIndexed(XMMATRIX worldMatrix, ConstantBuffer constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);

	constantBuffer.mWorld = worldMatrix;
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];

	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::PositionRenderingIndexed(ConstantBuffer constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);

	SetPosition(m_Position, constantBuffer, perObjectBuffer);

	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderIndexedWithGS(D3D_PRIMITIVE_TOPOLOGY topology)
{
	m_Context->IASetPrimitiveTopology(topology);
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
	m_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void D3DObject::RenderIndexedWithDynamicSRV(CComPtr<ID3D11ShaderResourceView>& textureRV)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &textureRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderInstanced(int numberOfInstances, CComPtr<ID3D11Buffer>& perInstanceBuffer)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->VSSetConstantBuffers(1, 1, &perInstanceBuffer.p);
	m_Context->DrawIndexedInstanced(m_Mesh.GetNumberOfIndices(), numberOfInstances, 0, 0, 0);
}

void D3DObject::RenderIndexedMulitexture(CComPtr < ID3D11ShaderResourceView > textureRVs[])
{
	ID3D11ShaderResourceView* tempArray[2];
	tempArray[0] = textureRVs[0];
	tempArray[1] = textureRVs[1];
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	//harcoding number of SRVs to 2
	m_Context->PSSetShaderResources(0, 2, tempArray);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderIndexedEmissive()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->PSSetShaderResources(1, 1, &m_SpecialTexture.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderIndexedEmissiveInstanced(int numberOfInstances, CComPtr<ID3D11Buffer>& perInstanceBuffer)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->PSSetShaderResources(1, 1, &m_SpecialTexture.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->VSSetConstantBuffers(1, 1, &perInstanceBuffer.p);
	m_Context->DrawIndexedInstanced(m_Mesh.GetNumberOfIndices(), numberOfInstances, 0, 0, 0);
}

void D3DObject::RenderIndexedEmissive(CComPtr<ID3D11ShaderResourceView> emissiveRV)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->PSSetShaderResources(1, 1, &emissiveRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::RenderIndexedTransparent()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//render back faces first
	m_Context->RSSetState(CCWcullMode);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
	//now render the forwad faces
	m_Context->RSSetState(CWcullMode);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);

}

void D3DObject::PositionRenderIndexedTransparent(ConstantBuffer constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	SetPosition(m_Position, constantBuffer, perObjectBuffer);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//render back faces first
	m_Context->RSSetState(CCWcullMode);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
	//now render the forwad faces
	m_Context->RSSetState(CWcullMode);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}


void D3DObject::RenderIndexedWithAO()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->PSSetShaderResources(1, 1, &m_SpecialTexture.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_Context->DrawIndexed(m_Mesh.GetNumberOfIndices(), 0, 0);
}

void D3DObject::UpdateVS(CComPtr<ID3D11VertexShader> vertexShader)
{
	m_VertexShader = vertexShader;
}

void D3DObject::UpdatePS(CComPtr<ID3D11PixelShader> pixelShader)
{
	m_PixelShader = pixelShader;
}

void D3DObject::UpdateGS(CComPtr < ID3D11GeometryShader > geoShader)
{
	m_GeometryShader = geoShader;
}

void D3DObject::UpdateTexture(string textureName)
{
	if (m_TextureRV)
	{
		m_TextureRV.Release();
		m_TextureRV = nullptr;
	}
	textureName = "Assets\\" + textureName + ".dds";
	std::wstring widestr = std::wstring(textureName.begin(), textureName.end());
	const wchar_t* widecstr = widestr.c_str();
	HRESULT hr = CreateDDSTextureFromFile(m_Device, widecstr, nullptr, &m_TextureRV);
}

void D3DObject::UpdateTexture(CComPtr < ID3D11ShaderResourceView> &textureRV)
{
	m_TextureRV = textureRV;
}

void D3DObject::Scale(float scale, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixScaling(scale, scale, scale)*XMMatrixTranspose(constantBuffer.mWorld);
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

void D3DObject::Scale(float scaleX, float scaleY, float scaleZ, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixScaling(scaleX, scaleY, scaleZ)*XMMatrixTranspose(constantBuffer.mWorld);
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

//Will set the position of the object
void D3DObject::SetPosition(XMVECTOR position, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	if (XMVector3Equal(position, XMVECTOR{ 0,0,0,0 }))
	{
		constantBuffer.mWorld = XMMatrixIdentity();
	}
	else
	{
		constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position));
	}
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

void D3DObject::SetPosition(XMMATRIX position, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixTranspose((position));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];

}

void D3DObject::SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr<ID3D11Buffer>& perObjectBuffer, float factorX, float factorY, float factorZ, bool inDegrees)
{
	if (!inDegrees)
	{
		constantBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationX(factorX)*XMMatrixRotationY(factorY)*XMMatrixRotationZ(factorZ)*(XMMatrixTranslationFromVector(position)));
		m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
		m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
	}
	else
	{
		constantBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationX(XMConvertToRadians(factorX))*XMMatrixRotationY(XMConvertToRadians(factorY))*XMMatrixRotationZ(XMConvertToRadians(factorZ))*(XMMatrixTranslationFromVector(position)));
		m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
		m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
	}
}

void D3DObject::SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr < ID3D11Buffer >& perObjectBuffer, float factor)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationY(factor)*(XMMatrixTranslationFromVector(position)));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

void D3DObject::SetLocalRotation(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr < ID3D11Buffer >& perObjectBuffer, float factorY, float factorX)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixRotationY(factorY)*XMMatrixRotationX(factorX)*(XMMatrixTranslationFromVector(position)));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

void D3DObject::SetRotatingPosition(XMVECTOR position, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer, float rotationMatrixFactor)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position)*XMMatrixRotationY(rotationMatrixFactor));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];

}

void D3DObject::SetRotatingPosition(XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr < ID3D11Buffer >& perObjectBuffer, float rotationMatrixYFactor, float rotationMatrixXFactor)
{
	constantBuffer.mWorld = XMMatrixTranspose(XMMatrixTranslationFromVector(position)*XMMatrixRotationY(rotationMatrixYFactor)*XMMatrixRotationX(rotationMatrixXFactor));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

void D3DObject::Rotate(XMMATRIX rotationMatrix, ConstantBuffer &constantBuffer, CComPtr < ID3D11Buffer> &perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixTranspose(rotationMatrix*XMMatrixTranspose(constantBuffer.mWorld));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

void D3DObject::RotateAndMove(XMMATRIX rotationMatrix, XMVECTOR position, ConstantBuffer & constantBuffer, CComPtr<ID3D11Buffer>& perObjectBuffer)
{
	constantBuffer.mWorld = XMMatrixTranspose(rotationMatrix*XMMatrixTranslationFromVector(position));
	m_Context->UpdateSubresource(perObjectBuffer, 0, nullptr, &constantBuffer, 0, 0);
	m_Position = XMMatrixTranspose(constantBuffer.mWorld).r[3];
}

XMVECTOR D3DObject::GetPosition()
{
	return m_Position;
}

float D3DObject::GetDistanceFromCamera(XMVECTOR objPosition)
{
	m_DistanceFromCamera = XMVector3Dot(objPosition - m_Position, objPosition - m_Position).m128_f32[0];
	return m_DistanceFromCamera;
}

D3DObject::~D3DObject()
{
	m_VertexBuffer.Release();
	m_IndexBuffer.Release();
	if (m_TextureRV)
	{
		m_TextureRV.Release();
		m_TextureRV = 0;
	}
	
	m_Mesh.LateDestructor();
}
