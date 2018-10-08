struct Light
{
	float4 Position;
	float4 Color;
	float4 Direction;
	float4 Range;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;

	float4 time;
	float4 vOutputColor;
}

cbuffer LightBuffer : register (b1)
{
	Light lights[5];
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD1;
	float4 wPos : POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;
	float4 pointLightColor = 0;
	float4 directionalLigthColor = 0;
 	float4 baseTexture = txDiffuse.Sample(samLinear, input.Tex);

	if (all(baseTexture == float4(0, 0, 0, 0)))
	{
		baseTexture = float4(0.5f, 0.5f, 0.5f, 1);
	}

	finalColor = saturate(baseTexture);
	finalColor.a = 1;
	return finalColor;
}