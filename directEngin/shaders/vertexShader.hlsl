cbuffer CBuf {

	matrix transform;
};

struct VS_INPUT
{
    float3 pos : Position;
    float2 inTexCord : TEXCORD;
};

struct VS_OUTPUT
{
	float4 pos : SV_Position;
	float2 TexCord : TEXCORD;
};

VS_OUTPUT main(VS_INPUT input)
{

	VS_OUTPUT res;
    res.pos = mul(float4(input.pos, 1.0f), transform);
    res.TexCord = input.inTexCord;
	return res;
}
