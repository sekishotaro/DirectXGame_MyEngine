cbuffer cbuff0 : register(b0)
{
	matrix mat; // �R�c�ϊ��s��
	float3 color;		//�F
};

//cbuffer cbuff1 : register(b1)
//{
//	float3 color : packoffset(c0);		//�F
//}

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
	float4 svpos : SV_POSITION; // �V�X�e���p���_���W
	float3 normal :NORMAL; // �@���x�N�g��
	float2 uv  :TEXCOORD; // uv�l
};