#include "AdsShader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
};

//エントリーポイント
PSOutput main(VSOutput input)
{
	PSOutput output;

	//環境光
	float4 ambient = color;

	//拡散反射光
	float3 light = normalize(-lightPos);
	float intensity = saturate(dot(input.normal, -light));
	float4 diffuseColor = { intensity, intensity, intensity, 1 };  //1~0の範囲
	float4 diffuse = diffuseColor;

	//鏡面反射
	float3 ipos = { input.svpos.x, input.svpos.y, input.svpos.z };
	float3 eyeDir = normalize(cameraPos - ipos);
	float3 harfvec = normalize(lightPos + eyeDir);
	float specularIntensity = pow(saturate(dot(normalize(input.normal), harfvec)), 50);
	float4 specularColor = float4(1, 1, 1, 1);
	float4 specular = specularColor * specularIntensity;
	
	//色を合成
	output.target0 = ambient * diffuse + specular;
	return output;
}