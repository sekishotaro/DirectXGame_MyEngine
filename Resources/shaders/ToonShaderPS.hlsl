#include "ToonShader.hlsli"

//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
Texture2D<float4> tex : register(t0);
//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[
SamplerState smp : register(s0);

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

//�G���g���[�|�C���g
PSOutput main(VSOutput input)
{
	PSOutput output;

	//�e�N�X�`���\�}�b�s���O
	float4 texcolor = color;
	//Lambert����
	float3 light = normalize(-lightPos); //�E�����@�������C�g
	float intensity = saturate(dot(input.normal, -light));
	float4 diffuseColor = { smoothstep(0.3, 0.35, intensity), smoothstep(0.3, 0.35, intensity), smoothstep(0.3, 0.35, intensity), 1 };
	float4 diffuse = diffuseColor;

	float3 ipos = { input.svpos.x, input.svpos.y, input.svpos.z };
	float3 eyeDir = normalize(cameraPos - ipos);
	float3 harfvec = normalize(lightPos + eyeDir);
	
	float specularIntensity = pow(saturate(dot(normalize(input.normal), harfvec)), 50);
	float4 specularColor = float4(1, 1, 1, 1);
	float4 specular = specularColor * smoothstep(0.5, 0.55, specularIntensity);
	
	//�A�e�ƃe�N�X�`���̐F������
	output.target0 = specular + diffuse * texcolor;

	return output;
}