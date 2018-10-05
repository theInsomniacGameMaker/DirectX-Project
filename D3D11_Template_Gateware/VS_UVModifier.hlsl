

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
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
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = mul(input.Pos, World);
	output.wPos = output.Pos;
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Norm = mul(float4(input.Norm, 0), World).xyz;
	output.Tex = input.Tex;
	//Changing UVS according to the sin of time
	output.Tex = float2(input.Tex.x*abs(sin(time.x/10)), input.Tex.y*abs(cos(time.x/10)));
	return output;
}