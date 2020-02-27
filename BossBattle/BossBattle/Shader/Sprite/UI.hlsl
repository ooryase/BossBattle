// 定数バッファ(CPU側からの値受け取り場)
cbuffer global : register(b0)
{
	matrix View;
	matrix Projection;
};



cbuffer global3 : register(b2)
{
	matrix World;
};

    Texture2D gtexture : register(t0);             // 画像の受け取り

    SamplerState SSSampler : register(s0);   // サンプラ

 
// 頂点シェーダから出力される構造体
struct VS_OUTPUT {
    float4 Pos : SV_POSITION;
    float2 TEX_UV : TEXCOORD;
};
 
// 頂点シェーダ
VS_OUTPUT VS(float4 Pos : POSITION, float2 Tex : TEXCOORD) {
 
    VS_OUTPUT output;
    output.Pos = Pos;
    output.TEX_UV = Tex;
 
    return output;
}

//ジオメトリシェーダ
[maxvertexcount(3)]
void GS(triangle VS_OUTPUT input[3],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<VS_OUTPUT> Stream)
{
	[unroll]
	for (int i = 0; i < 3; i++)
	{
		Stream.Append(input[i]);
	}

	Stream.RestartStrip();
}

 
// ピクセルシェーダ
float4 PS(VS_OUTPUT input) : SV_Target{
    return gtexture.Sample(SSSampler, input.TEX_UV);
}