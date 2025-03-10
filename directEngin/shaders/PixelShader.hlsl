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

struct GBuffer
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
    float4 depth  : SV_Target2;
};


GBuffer main(VS_OUTPUT input) : SV_Target
{

    GBuffer output;
    float4 textureColor = ObjTexture.Sample(ObjSamplerState, input.TexCord);
    output.albedo = MaterialColor * textureColor;
    output.depth = float4(saturate(input.pos.z / input.pos.w), 0.0f, 0.0f, 1.0f);
    
    return output;
}
