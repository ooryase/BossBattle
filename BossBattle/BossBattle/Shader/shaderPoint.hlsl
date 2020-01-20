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
	matrix World;
	float4 Light;
	float4 Attenuation;
}
 
// 頂点シェーダ
PS_IN VS(VS_IN input)
{
	float4 inPos = float4(input.pos,1.0f);
	PS_IN output;
	output.wld = mul(inPos, World);
	output.pos = mul(output.wld, View);
	output.pos = mul(output.pos, Projection);
	output.nor = mul(float4(input.nor,1.0f), World);

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
    float3 dir;
    float  len;
    float  colD;
    float  colA;
    float  col;
 
    //点光源の方向
    dir = Light.xyz - input.wld.xyz;
 
    //点光源の距離
    len = length(dir);
 
    //点光源の方向をnormalize
    dir = dir / len;
 
    //拡散
    colD = saturate(dot(normalize(input.nor.xyz), dir));
    //減衰
    colA = saturate(1.0f / (Attenuation.x + Attenuation.y * len + Attenuation.z * len * len));
 
    col = colD * colA;

	int level = (int)(col / 0.2f);
	col = 1.0f / 4.0f * level;

    return float4(col, col, col, 1.0f);
}