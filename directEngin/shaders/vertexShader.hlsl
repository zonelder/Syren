struct VSOut {
	float3 color:Color;
	float4 pos:SV_Position;
};

cbuffer CBuf {

	row_major matrix transform;//берет транспонированную матрицу за основу трансформа. это надо переработать
};

VSOut  main(float2 pos : Position,float4 color:Color){
	VSOut vso;
	vso.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f),transform);
	vso.color = color;
	return vso;
}
