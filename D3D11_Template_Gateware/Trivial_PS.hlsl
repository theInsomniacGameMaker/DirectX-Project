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
	float4 wPos : POSITION;
};

float4 main(PS_INPUT input) : SV_Target
{
	float4 finalColor = 0;
	float4 pointLightColor = 0;
	float4 directionalLigthColor = 0;
	float4 newPosition = pLight.pos;
	//float4 newPosition = mul(pLight.pos, sin(time)*5);
	float4 baseTexture = txDiffuse.Sample(samLinear, input.Tex);

	if (all(baseTexture == float4(0, 0, 0, 0)))
	{
		baseTexture = float4(0.5f, 0.5f, 0.5f, 1);
	}

	float3 lightToPixelVec = newPosition - input.wPos;


	float howMuchLight = saturate(dot(normalize(lightToPixelVec), input.Norm));

	pointLightColor = howMuchLight * baseTexture* pLight.diffuse;
	pointLightColor *= (1.0 - saturate(length(newPosition - input.wPos) / pLight.range));



	//do NdotL lighting for 2 directional lights
	for (int i = 0; i < 2; i++)
	{
		directionalLigthColor += saturate(dot((float3)vLightDir[i], input.Norm) * vLightColor[i] * baseTexture);
	}
	finalColor = saturate(directionalLigthColor + pointLightColor);
	finalColor.a = 1;
	return finalColor;
}