#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION,float3 normal : NORMAL, float2 uv : TEXCOORD)
{
	VSOutput output; // ピクセルシェーダーに渡す値
	
	output.svpos = mul(mat, pos); //通常カメラで見える見え方の計算
	output.normal = normal;
	output.uv = uv;
	return output;
}
