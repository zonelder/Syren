cbuffer CBuf {

	matrix transform;
};

struct VS_OUTPUT
{
	float4 pos : SV_Position;
	float2 TexCord : TEXCORD;
};

VS_OUTPUT main(float3 pos : Position,float2 inTexCord:TEXCORD){

	VS_OUTPUT res;
	res.pos = mul(float4(pos, 1.0f), transform);
	res.TexCord = inTexCord;
	return res;
}
