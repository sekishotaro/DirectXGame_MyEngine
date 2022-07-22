#include "AdsShader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
};

//�G���g���[�|�C���g
PSOutput main(VSOutput input)
{
	PSOutput output;

	//����
	float4 ambient = color;

	//�g�U���ˌ�
	float3 light = normalize(-lightPos);
	float intensity = saturate(dot(input.normal, -light));
	float4 diffuseColor = { intensity, intensity, intensity, 1 };  //1~0�͈̔�
	float4 diffuse = diffuseColor;

	//���ʔ���
	float3 ipos = { input.svpos.x, input.svpos.y, input.svpos.z };
	float3 eyeDir = normalize(cameraPos - ipos);
	float3 harfvec = normalize(lightPos + eyeDir);
	float specularIntensity = pow(saturate(dot(normalize(input.normal), harfvec)), 50);
	float4 specularColor = float4(1, 1, 1, 1);
	float4 specular = specularColor * specularIntensity;
	
	//�F������
	output.target0 = ambient * diffuse + specular;
	return output;
}