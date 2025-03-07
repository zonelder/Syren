cbuffer MatBuff : register(b1)
{
    float4 MaterialColor;
}

struct VS_OUTPUT
{
    float4 pos : SV_Position;
};

struct GBuffer
{
    float4 albedo : SV_Target0;
    float4 normal : SV_Target1;
    float4 depth : SV_Target2;
};


GBuffer main(VS_OUTPUT input)
{
    GBuffer output;
    output.albedo = MaterialColor;
    output.depth = float4(saturate(input.pos.z/input.pos.w), 0.0f, 0.0f, 1.0f);
    return output;
}
