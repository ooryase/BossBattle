struct VS_IN
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
};

struct GS_IN
{
	float4 pos : SV_POSITION;
	float edge : COLOR;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
	float edge : COLOR;
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
	float4 Plyer;
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
GS_IN VS(VS_IN input)
{
	float4 inPos = float4(input.pos,1.0f);
	GS_IN output;
	output.pos = mul(inPos, World);
	output.pos = mul(output.pos, View);
	output.pos = mul(output.pos, Projection);
	output.edge = 1.0f;

	return output;
}

//ジオメトリシェーダ
[maxvertexcount(10)]
void GS(lineadj GS_IN input[4],
	uint primID : SV_PrimitiveID,
	inout LineStream<PS_IN> Stream)
{
	float3 v0 = input[1].pos.xyz/input[1].pos.w;
	float3 v1 = input[2].pos.xyz/input[2].pos.w;
	float3 va = input[0].pos.xyz/input[0].pos.w;
	float3 vb = input[3].pos.xyz/input[3].pos.w;
	
	float3 v0a = v0-va;
	float3 v1a = v1-va;
	float3 v0b = v0-vb;
	float3 v1b = v1-vb;
	if( 0 > cross(v0a,v1a).z * cross(v1b,v0b).z )
	{
		PS_IN vo0,vo1;
		float4 p0 = input[1].pos;
		float4 p1 = input[2].pos;
		vo0.pos = p0;
		vo1.pos = p1;
		vo0.edge = input[1].edge;
		vo1.edge = input[2].edge;
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		//一本だと細いので上下左右にずらして描画
		const float px = 0.8*p0.w/640.0;
		const float py = 0.8*p1.w/480.0;

		vo0.pos = p0 + float4(px,0,0,0);
		vo1.pos = p1 + float4(px,0,0,0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		vo0.pos = p0 + float4(-px,0,0,0);
		vo1.pos = p1 + float4(-px,0,0,0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();
		
		vo0.pos = p0 + float4(0,py,0,0);
		vo1.pos = p1 + float4(0,py,0,0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();

		vo0.pos = p0 + float4(0,-py,0,0);
		vo1.pos = p1 + float4(0,-py,0,0);
		Stream.Append(vo0);
		Stream.Append(vo1);
		Stream.RestartStrip();
		
	}

	Stream.RestartStrip();
}
 
// ピクセルシェーダ
float4 PS(PS_IN input) : SV_Target
{
	return float4(1.0f,1.0f,1.0f,1.0f);//0.25*input.edge);

	//float3 nor;
	//nor = mul(input.nor, World).xyz;
	//nor = normalize(nor);

	//float col;
	//col = saturate(dot(normalize((float3)Light),normalize(nor)));
	//col = saturate(dot(nor, (float3)Light));
	///return float4(col,col,col,1.0f);

}