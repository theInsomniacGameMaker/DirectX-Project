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
	float4 time;
	float4 vOutputColor;
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
	//float3 TPos: TEXCOORDP;
};

//--------------------------------------------------------------------------------------
struct SKYMAP_VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Tex : TEXCOORD1;
};

SKYMAP_VS_OUTPUT main(VS_INPUT input)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(input.Pos, View);
	output.Pos = mul(input.Pos, Projection);
	output.Tex = input.Pos;
	return output;
}