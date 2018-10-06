// format of output verticies
// these could be going to the
// rasterizer the (SO)stage or both.
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
[maxvertexcount(3)] // max vertex data to be output (limit 1024 total scalars)
void main(triangle GSOutput input[3], inout TriangleStream< GSOutput > output)
{
	// red green and blue vertex
	GSOutput verts[3] =
	{
	float4(1,0,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,1,0,1), float3(0,0,0),float2(0,0),float4(0,0,0,0),
	float4(0,0,1,1), float3(0,0,0),float2(0,0),float4(0,0,0,0)
	};
	// bottom left
	verts[0].Pos.xyz = input[0].Pos.xyz;
	verts[0].Pos.x -= 0.5f;
	// bottom right
	verts[2].Pos = verts[0].Pos;
	verts[2].Pos.x += 1.0f;
	// top center
	verts[1].Pos.xyz = input[1].Pos.xyz;
	// prep triangle for rasterization
	float4x4 mVP = mul(View, Projection);
	for (uint i = 0; i < 3; ++i)
		verts[i].Pos = mul(verts[i].Pos, mVP);
	// send verts to the rasterizer
	output.Append(verts[0]);
	output.Append(verts[1]);
	output.Append(verts[2]);
}
