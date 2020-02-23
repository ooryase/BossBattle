struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
};



struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 wld : POSITION;
	float4 nor : NORMAL;
};


// 定数バッファ(CPU側からの値受け取り場)
cbuffer CBuffer : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer CBuffer2 : register(b1)
{
	float3 Directional;
	int LightCount;
	float4 LightPos[8];
	float4 LightColor[8];
	float4 LightAttenuation[8];
}

cbuffer CBuffer3 : register(b2)
{
	matrix World;
	float4 Power;
}
 
// 頂点シェーダ
PS_IN VS(VS_IN input)
{
	float4 inPos = float4(input.pos,1.0f);
	PS_IN output;
	output.wld = mul(inPos, World);
	output.pos = mul(output.wld, View);
	output.pos = mul(output.pos, Projection);
	output.nor = mul(float4(input.nor, 1.0f), World);

	return output;
}

//ジオメトリシェーダ
[maxvertexcount(3)]
void GS(triangle PS_IN input[3],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<PS_IN> Stream)
{
	float3 edge1 = (input[1].pos.xyz - input[0].pos.xyz);
	float3 edge2 = (input[2].pos.xyz - input[0].pos.xyz);
	float3 normal = normalize(cross(edge1, edge2));

	[unroll]
	for (int i = 0; i < 3; i++)
	{
		input[i].nor = float4(normal,1.0f);
		Stream.Append(input[i]);
	}

	Stream.RestartStrip();
}

 
// ピクセルシェーダ
float4 PS(PS_IN input) : SV_Target
{
    float3 dir;
    float  len;
    float  col;

	float3 center = float3(0.5f,0.5f,input.wld.z);
 
    //点光源の方向
    dir = center.xyz - input.wld.xyz;
 
    //点光源の距離
    len = length(dir);
 
    //点光源の方向をnormalize
    dir = dir / len;
 
 
    col = Power.x - len / 20.0f;

	return float4(col, col, col, col);
}