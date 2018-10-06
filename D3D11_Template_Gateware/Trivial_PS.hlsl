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

	for (int i = 0; i < 5; i++)
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
		else if (lights[i].Position.w == 3)
		{
			float3 lightToPixelVec = normalize(lights[i].Position - input.wPos);
			float surfaceRatio = saturate(dot(-lightToPixelVec, float3(lights[i].Direction.x, lights[i].Direction.y, lights[i].Direction.z)));
			float spotFactor = (surfaceRatio > lights[i].Range.y) ? 1 : 0;
			float lightRatio = saturate(dot(lightToPixelVec, input.Norm));
			float3 spotLightColor = spotFactor*lightRatio*lights[i].Color*baseTexture;
			//spotLightColor *= (1.0 - saturate(length(lights[i].Position - input.wPos) / lights[i].Range.z));
			spotLightColor *= (1.0 - saturate((lights[i].Range.x - surfaceRatio) / (lights[i].Range.x - lights[i].Range.y)));
			finalColor += float4(spotLightColor,1);
		}
	}	
	finalColor = saturate(finalColor);
	finalColor.a = 1;
	return finalColor;
}