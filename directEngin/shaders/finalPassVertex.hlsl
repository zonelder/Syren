

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD0;
};


VS_OUTPUT main(uint id : SV_VertexID)
{
    VS_OUTPUT output;
    float2 positions[4] =
    {
        float2(-1.0, -1.0),
        float2(-1.0, 1.0),
        float2(1.0, -1.0),
        float2(1.0, 1.0)

    };
    
    float2 uvs[4] =
    {
        float2(0.0, 1.0),
        float2(0.0, 0.0),
        float2(1.0, 1.0),
        float2(1.0, 0.0)
    };
    
    output.pos = float4(positions[id],0.0f,1.0f);
    output.texCoord = uvs[id];

    return output;
}