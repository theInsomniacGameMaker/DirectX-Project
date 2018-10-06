static const int j = 1, k = 2, l=5;

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 time;
	float4 vOutputColor;
}

cbuffer ConstantBuffer : register (b1)
{
	matrix worldArray[10];
}

struct VS_INPUT
{
	float4 Pos : POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD0;
	float4 Tan: TANGENT;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD1;
	float4 wPos : POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//position.x += a * sin(k * position.y + f * time);
	input.Pos.y += j * sin((k*input.Pos.x + l * time.x)/5.0f);
	output.Pos = mul(input.Pos, World);
	output.wPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(float4(input.Norm, 0), World).xyz;
	output.Tex = input.Tex;
	return output;
}