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
	float4 Directional;
	float4 Player;
	float4 PColor;
	float4 PAttenuation;
	int ELCount;
	float4 Enemy[8];
	float4 EColor[8];
	float4 EAttenuation[8];
}

cbuffer CBuffer3 : register(b2)
{
	matrix World;
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
    float  colD;
    float  colA;
    float  col;
 
    //点光源の方向
    dir = Player.xyz - input.wld.xyz;
 
    //点光源の距離
    len = length(dir);
 
    //点光源の方向をnormalize
    dir = dir / len;
 
    //拡散
    colD = saturate(dot(normalize(input.nor.xyz), dir));
    //減衰
    colA = saturate(1.0f / (PAttenuation.x + PAttenuation.y * len + PAttenuation.z * len * len));
 
    col = colD * colA;

	//平行光源の光を加算
	col += saturate(dot(input.nor.xyz, (float3)Directional)) * 0.6f;

	//階調化
	int level = (int)(col / 0.2f);
	col = 1.0f / 4.0f * level;

    return float4(col, col, col, 1.0f);
}