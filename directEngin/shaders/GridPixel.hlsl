

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};


float4 main(VS_OUTPUT input) : SV_Target
{
    return input.color;
}
