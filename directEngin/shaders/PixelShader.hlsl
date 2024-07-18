cbuffer MatBuff : register(b1)
{
    float4 MaterialColor;
}



Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 pos : SV_Position;
	float2 TexCord : TEXCORD;
};


float4 main(VS_OUTPUT input) : SV_Target{

    float4 textureColor = ObjTexture.Sample(ObjSamplerState, input.TexCord);
    return MaterialColor*textureColor;
}
