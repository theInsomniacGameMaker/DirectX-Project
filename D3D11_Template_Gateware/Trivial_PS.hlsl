cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 vLightDir[2];
	float4 vLightColor[2];
	float4 vOutputColor;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;

	//do NdotL lighting for 2 lights
	for (int i = 0; i < 2; i++)
	{
		finalColor += saturate(dot((float3)vLightDir[i],input.Norm) * vLightColor[i]);
	}
	finalColor *= txDiffuse.Sample(samLinear, input.Tex);
	finalColor.a = 1;
	return finalColor;
}