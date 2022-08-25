cbuffer cbuff0 : register(b0)
{
	matrix mat; // ３Ｄ変換行列
	float4 color;		//色
};


// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
};