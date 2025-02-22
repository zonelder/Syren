cbuffer MatBuff : register(b1)
{
    float4 MaterialColor;
}



struct VS_OUTPUT
{
    float4 pos : SV_Position;
};


float4 main(VS_OUTPUT input) : SV_Target
{
    return MaterialColor;
}
