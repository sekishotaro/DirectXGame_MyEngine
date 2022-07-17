#include "ToonShader.hlsli"

//0番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
//0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

//エントリーポイント
PSOutput main(VSOutput input)
{
	PSOutput output;

	//テクスチャ―マッピング
	float4 texcolor = color;
	//Lambert反射
	float3 light = normalize(-lightPos); //右下奥　向きライト
	float intensity = saturate(dot(input.normal, -light));
	float4 diffuseColor = { smoothstep(0.3, 0.35, intensity), smoothstep(0.3, 0.35, intensity), smoothstep(0.3, 0.35, intensity), 1 };
	float4 diffuse = diffuseColor;

	float3 ipos = { input.svpos.x, input.svpos.y, input.svpos.z };
	float3 eyeDir = normalize(cameraPos - ipos);
	float3 harfvec = normalize(lightPos + eyeDir);
	
	float specularIntensity = pow(saturate(dot(normalize(input.normal), harfvec)), 50);
	float4 specularColor = float4(1, 1, 1, 1);
	float4 specular = specularColor * smoothstep(0.5, 0.55, specularIntensity);
	
	//陰影とテクスチャの色を合成
	output.target0 = specular + diffuse * texcolor;

	return output;
}