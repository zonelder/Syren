
cbuffer FinalPassCB : register(b0)
{
    int selectedOutput; //0 - albedo,1- normals,2- depth
}


Texture2D albedoTexture : register(t0);
Texture2D normalTexture : register(t1);
Texture2D DepthTexture : register(t2);
SamplerState FinalSampler : register(s0);

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float2 texCoord : TEXCOORD0;
};


float4 main(VS_OUTPUT input ) : SV_Target
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

    if(selectedOutput == 0)
    {
        
        color = albedoTexture.Sample(FinalSampler, input.texCoord);
    }
    else if(selectedOutput == 1)
    {
        float4 normal = normalTexture.Sample(FinalSampler, input.texCoord);
    }
    else if(selectedOutput == 2)
    {
        float depth = DepthTexture.Sample(FinalSampler, input.texCoord).x;
        float r = depth * 255.0;
        float g = frac(r) * 255.0;
        float b = frac(g) * 255.0;
    
        color = float4(floor(r) / 255.0, floor(g) / 255.0, floor(b) / 255.0, 1.0);
    }
    else
    {
        
        color = albedoTexture.Sample(FinalSampler, input.texCoord);
    }
    return color;
}