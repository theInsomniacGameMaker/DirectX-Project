// format of output verticies
// these could be going to the
// rasterizer the (SO)stage or both.
struct GSInput
{
	float4 Pos : POSITION0;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD1;
	float4 wPos : POSITION1;
};

struct GSOutput
{
	float4 Pos : SV_POSITION;
	float3 Norm : NORMAL;
	float2 Tex : TEXCOORD1;
	float4 wPos : POSITION;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
	float4 time;
	float4 vOutputColor;
}
// convert each incoming world-space line segment into a projected triangle. 
[maxvertexcount(4)] // max vertex data to be output (limit 1024 total scalars)
void main(point GSInput input[1], inout TriangleStream< GSOutput > output)
{
	// red green and blue vertex
	GSOutput verts[4] =
	{
	float4(1,0,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,1,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,0,1,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,0,1,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	};

	verts[0].Pos = input[0].Pos;
	verts[0].Pos.x -= 0.5f;
	verts[0].Pos.y += 0.5f;

	verts[1].Pos = input[0].Pos;
	verts[1].Pos.x += 0.5f;
	verts[1].Pos.y += 0.5f;

	verts[2].Pos = input[0].Pos;
	verts[2].Pos.x -= 0.5f;
	verts[2].Pos.y -= 0.5f;

	verts[3].Pos = input[0].Pos;
	verts[3].Pos.x += 0.5f;
	verts[3].Pos.y -= 0.5f;

	for (int i = 0; i < 4; i++)
	{
		verts[i].Pos = mul(verts[i].Pos, World);
		verts[i].Pos = mul(verts[i].Pos, View);
		verts[i].Pos = mul(verts[i].Pos, Projection);
	}

	// send verts to the rasterizer
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
	output.Append(verts[3]);
}
