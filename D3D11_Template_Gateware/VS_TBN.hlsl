

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;

	float4 time;
	float4 vOutputColor;
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
	float4 tangent: TANGENT;
	float4 binormal: BINORMALS;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);
	output.wPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(float4(input.Norm, 0), World).xyz;
	output.tangent = mul(float4(input.Tan.xyz*input.Tan.w, 0.0f), World);
	output.binormal = mul(float4(cross(input.Norm.xyz, input.Tan.xyz), 0.0f), World);
	output.Tex = input.Tex;
	return output;
}