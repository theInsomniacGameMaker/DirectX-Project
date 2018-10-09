TextureCube SkyMap : register(t0);
SamplerState samLinear : register(s0);

struct SKYMAP_PS_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm: NORMAL;
	float3 Tex : TEXCOORD1;
	float4 wPos: POSITION;
};

float4 main(SKYMAP_PS_INPUT input) : SV_Target
{
	return SkyMap.Sample(samLinear, input.Tex);
}