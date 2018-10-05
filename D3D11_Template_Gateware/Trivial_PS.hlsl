struct PointLight
{
	float4 pos;
	float  range;
	float3 att;
	float4 diffuse;
};

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
	float4 vLightDir[2];
	float4 vLightColor[2];
	PointLight pLight;
	float time;
	float4 vOutputColor;
}

cbuffer LightBuffer : register (b1)
{
	Light lights[3];
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
	float4 baseTexture = txDiffuse.Sample(samLinear, input.Tex);

	if (all(baseTexture == float4(0, 0, 0, 0)))
	{
		baseTexture = float4(0.5f, 0.5f, 0.5f, 1);
	}

	for (int i = 0; i < 3; i++)
	{
		if (lights[i].Position.w == 1)
		{
			finalColor += saturate(dot((float3)lights[i].Direction, input.Norm) * lights[i].Color * baseTexture);
		}
		else if (lights[i].Position.w == 2)
		{
			float3 lightToPixelVec = lights[i].Position - input.wPos;
			float howMuchLight = saturate(dot(normalize(lightToPixelVec), input.Norm));
			pointLightColor = howMuchLight * baseTexture* lights[i].Color;
			pointLightColor *= (1.0 - saturate(length(lights[i].Position - input.wPos) / lights[i].Range.x));
			finalColor += saturate(pointLightColor);
		}
	}	
	finalColor = saturate(finalColor);
	finalColor.a = 1;
	return finalColor;
}