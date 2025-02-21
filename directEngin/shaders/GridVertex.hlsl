cbuffer CBuf 
{
	matrix transform;
};

struct VS_INPUT
{
    float3 pos : Position;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_Position;
	float4 color : COLOR;
};

VS_OUTPUT main(VS_INPUT input)
{

	VS_OUTPUT res;
    res.pos = mul(float4(input.pos, 1.0f), transform);
    res.color = input.color;
  
	return res;
}
