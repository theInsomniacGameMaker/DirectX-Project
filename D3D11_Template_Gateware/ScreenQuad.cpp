#include "ScreenQuad.h"



ScreenQuad::ScreenQuad()
{
}

ScreenQuad::ScreenQuad(CComPtr<ID3D11Device>& myDevice, CComPtr<ID3D11DeviceContext>& myContext, CComPtr<ID3D11VertexShader>& vertexShader, CComPtr<ID3D11PixelShader>& pixelShader, CComPtr<ID3D11GeometryShader>& geoShader, XMFLOAT3 positions[])
{
	m_Device = myDevice;
	m_Context = myContext;
	m_VertexShader = vertexShader;
	m_PixelShader = pixelShader;
	m_GeometryShader = geoShader;

	screenQuadVertices = new SimpleVertex[6];

	screenQuadVertices[0].Pos = positions[0];
	screenQuadVertices[1].Pos = positions[1];
	screenQuadVertices[2].Pos = positions[2];
	screenQuadVertices[3].Pos = positions[1];
	screenQuadVertices[5].Pos = positions[2];
	screenQuadVertices[4].Pos = positions[3];

	screenQuadVertices[0].Tex = XMFLOAT2(0, 0);
	screenQuadVertices[1].Tex = XMFLOAT2(1, 0);
	screenQuadVertices[2].Tex = XMFLOAT2(0, 1);
	screenQuadVertices[3].Tex = XMFLOAT2(1, 0);
	screenQuadVertices[5].Tex = XMFLOAT2(0, 1);
	screenQuadVertices[4].Tex = XMFLOAT2(1, 1);

	D3D11_BUFFER_DESC bd = {};
	D3D11_SUBRESOURCE_DATA InitData = {};
	stride[0] = sizeof(SimpleVertex);
	offset[0] = 0;

	//Desc Vertex Buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	//Subresource
	InitData.pSysMem = screenQuadVertices;

	//Create Vertex Buffer
	m_Device->CreateBuffer(&bd, &InitData, &m_VertexBuffer);

}

void ScreenQuad::Render()
{
	m_Context->VSSetShader(m_VertexShader, nullptr, 0);
	m_Context->PSSetShader(m_PixelShader, nullptr, 0);
	m_Context->GSSetShader(m_GeometryShader, nullptr, 0);
	m_Context->PSSetShaderResources(0, 1, &m_TextureRV.p);
	m_Context->IASetVertexBuffers(0, 1, &m_VertexBuffer.p, stride, offset);
	m_Context->Draw(6, 0);
}

void ScreenQuad::UpdateTexture(CComPtr<ID3D11ShaderResourceView>& textureRV)
{
	m_TextureRV = textureRV;
}


ScreenQuad::~ScreenQuad()
{
	delete[] screenQuadVertices;
	m_VertexBuffer.Release();
	m_IndexBuffer.Release();
	m_VertexShader.Release();
	m_PixelShader.Release();
	m_TextureRV.Release();
}
