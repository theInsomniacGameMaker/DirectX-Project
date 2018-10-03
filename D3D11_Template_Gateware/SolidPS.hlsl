struct PointLight
{
	float4 pos;
	float  range;
	float3 att;
	float4 diffuse;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
	PointLight pLight;
	float time;
	float4 vOutputColor;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD1;
	float3 TPos : TEXCOORD3;
};

float4 main(PS_INPUT input) : SV_Target
{
	return vOutputColor;
}