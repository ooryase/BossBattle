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
	output.pos = inPos;
	output.nor = mul(float4(input.nor, 1.0f), World);

	return output;
}

//ジオメトリシェーダ
[maxvertexcount(3)]
void GS(triangle PS_IN input[3],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<PS_IN> Stream)
{
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		Stream.Append(input[i]);
	}

	Stream.RestartStrip();
}

 
// ピクセルシェーダ
float4 PS(PS_IN input) : SV_Target
{
    float2 dir;
    float  lenSquare;
    float  col;

	float2 center = float2(0.5f,0.5f);
 
    dir = center.xy - input.pos.xy;
    //中心からの距離
    lenSquare = dir.x * dir.x + dir.y * dir.y;
 
 
 
    col = Power.x + Power.y * lenSquare * 2.0f;

	return float4(col, col, col, 1.0f);
}