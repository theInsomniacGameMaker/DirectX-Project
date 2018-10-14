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
[maxvertexcount(6)] // max vertex data to be output (limit 1024 total scalars)
void main(triangle GSInput input[3], inout TriangleStream< GSOutput > output)
{
	// red green and blue vertex
	GSOutput verts[3] =
	{
	float4(1,0,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,1,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,0,1,1), float3(0,0,0),float2(0,0),float4(0,0,0,0)
	};

	GSOutput oldVerts[3] =
	{
	float4(1,0,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,1,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,0,1,1), float3(0,0,0),float2(0,0),float4(0,0,0,0)
	};

	verts[0].Pos = input[0].Pos;
	verts[1].Pos = input[1].Pos;
	verts[2].Pos = input[2].Pos;

	/*oldVerts[0].Pos = input[0].Pos;
	oldVerts[1].Pos = input[1].Pos;
	oldVerts[2].Pos = input[2].Pos;*/

	for (int i = 0; i < 3; i++)
	{
		verts[i].Pos = mul(verts[i].Pos, World);
		verts[i].wPos = verts[i].Pos;

		oldVerts[i].Pos = verts[i].Pos;
		oldVerts[i].wPos = verts[i].wPos;
		oldVerts[i].Norm = mul(input[i].Norm, World);
		oldVerts[i].Tex = input[i].Tex;

		oldVerts[i].Pos.x *= -1;
		oldVerts[i].wPos.x *= -1;
		oldVerts[i].Norm.x *= -1;

		oldVerts[i].Pos = mul(oldVerts[i].Pos, View);
		oldVerts[i].Pos = mul(oldVerts[i].Pos, Projection);

		verts[i].Pos = mul(verts[i].Pos, View);
		verts[i].Pos = mul(verts[i].Pos, Projection);
		verts[i].Norm = mul(input[i].Norm,World);
		verts[i].Tex = input[i].Tex;

	}


	// send verts to the rasterizer
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);

	output.RestartStrip();

	output.Append(oldVerts[2]);
	output.Append(oldVerts[1]);
	output.Append(oldVerts[0]);
}
