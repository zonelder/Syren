cbuffer Cbuf {
	float4 face_color[6];
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 pos : SV_Position;
	float2 TexCord : TEXCORD;
};


float4 main(VS_OUTPUT input) : SV_Target{

	return ObjTexture.Sample( ObjSamplerState, input.TexCord );
}
