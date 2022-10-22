cbuffer cbuff0 : register(b0)
{
	matrix mat; // ３Ｄ変換行列
};

cbuffer cbuff2 : register(b1)
{
	matrix matr;			//3d変換行列
	matrix shadow;		//影行列
	matrix eye;			//視点
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float3 normal :NORMAL; // 法線ベクトル
	float2 uv  :TEXCOORD; // uv値
};
