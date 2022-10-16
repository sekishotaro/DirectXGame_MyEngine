#include "Model.h"

#include <cassert>
#include <Windows.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// �ÓI�����o�ϐ��̎���
/// </summary>
const std::string Model::baseDirectory = "Resources/";
ID3D12Device* Model::device = nullptr;
UINT Model::descriptorHandleIncrementSize = 0;

Model *Model::LoadFromOBJ(const std::string &modelname)
{
	//�V����Model�^�̃C���X�^���X�̃��������m��
	Model* model = new Model();
	
	//�ǂݍ���
	model->Initialize(modelname);

	return model;
}

void Model::SetDevice(ID3D12Device* device)
{
	// �ď������`�F�b�N
	assert(!Model::device);

	Model::device = device;

	// ���b�V���̐ÓI������
	Mesh::StaticInitialize(device);
}

void Model::Initialize(const std::string& modelname)
{
	LoadFormOBJInternal(modelname);

	// ���b�V���̃}�e���A���`�F�b�N
	for (auto& m : meshes) {
		// �}�e���A���̊��蓖�Ă��Ȃ�
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// �f�t�H���g�}�e���A���𐶐�
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// �f�t�H���g�}�e���A�����Z�b�g
			m->SetMaterial(defaultMaterial);
		}
	}

	//�o�b�t�@����
	//model->CreateBuffers();
	// ���b�V���̃o�b�t�@����
	for (auto& m : meshes) {
		m->CreateBuffers();
	}

	// �}�e���A���̐��l��萔�o�b�t�@�ɔ��f
	for (auto& m : materials) {
		m.second->Update();
	}

	//�f�X�N���v�^�q�[�v�̐���
	InitializeDescriptorHeap();

	// �e�N�X�`���̓ǂݍ���
	LoadTexture();
}

void Model::LoadFormOBJInternal(const std::string &modelname)
{
	//.obj�t�@�C�����J��
	const string filename = modelname + ".obj";                  //"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/"; //"Resources/triangle_mat/"
	
	//�t�@�C���X�g���[��
	std::ifstream file;
	file.open(directoryPath + filename);                         //"Resources/triangle_mat/triangle_mat.obj"
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	name = modelname;

	// ���b�V������
	meshes.emplace_back(new Mesh);
	Mesh* mesh = meshes.back();
	int indexCountTex = 0;
	int indexCountNoTex = 0;

	vector<XMFLOAT3> positions;    //���_���W
	vector<XMFLOAT3> normals;      //�@���x�N�g��
	vector<XMFLOAT2> texcoords;    //�e�N�X�`��UV
	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		//���p�X�y�[�X�D�؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪������mtllib�Ȃ�}�e���A��
		if (key == "mtllib")
		{
			//�}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			//�}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}
		// �擪������g�Ȃ�O���[�v�̊J�n
		if (key == "g") {

			// �J�����g���b�V���̏�񂪑����Ă���Ȃ�
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
				// ���_�@���̕��ςɂ��G�b�W�̕�����
				//if (smoothing)
				//{
				//	mesh->CalculateSmoothedVertexNormals();
				//}
				// ���̃��b�V������
				meshes.emplace_back(new Mesh);
				mesh = meshes.back();
				indexCountTex = 0;
			}

			// �O���[�v���ǂݍ���
			string groupName;
			line_stream >> groupName;

			// ���b�V���ɖ��O���Z�b�g
			mesh->SetName(groupName);
		}

		//�擪������v�Ȃ璸�_���W
		if (key == "v")
		{
			//X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//���W�f�[�^�ɒǉ�
			positions.emplace_back(position);
			//���_�f�[�^�ɒǉ�
		}

		//�擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			//U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V�������]
			texcoord.y = 1.0f - texcoord.y;
			//�e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		//�擪������vn�Ȃ�@���x�N�g��
		if (key == "vn")
		{
			//X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//�@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		// �擪������usemtl�Ȃ�}�e���A�������蓖�Ă�
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr) {
				// �}�e���A���̖��ǂݍ���
				string materialName;
				line_stream >> materialName;

				// �}�e���A�����Ō������A�}�e���A�������蓖�Ă�
				auto itr = materials.find(materialName);
				if (itr != materials.end()) {
					mesh->SetMaterial(itr->second);
				}
			}
		}

		//�擪������f�Ȃ�|���S�� (�O�p�`)
		if (key == "f")
		{
			int faceIndexCount = 0;
			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			int countNum = 0;
			while (getline(line_stream, index_string, ' '))
			{
				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				//���_�ԍ�
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); //�X���b�V�����΂�
				//�}�e���A���A�e�N�X�`��������ꍇ
				if (material && material->textureFilename.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); //�X���b�V�����΂�
					index_stream >> indexNormal;
					//���_�f�[�^�̒ǉ�
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);
					// �G�b�W�������p�̃f�[�^��ǉ�
					//if (smoothing)
					//{
					//	mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
					//}
				}
				else
				{
					char c;
					index_stream >> c;
					// �X���b�V��2�A���̏ꍇ�A���_�ԍ��̂�
					if (c == '/')
					{
						// ���_�f�[�^�̒ǉ�
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
					else
					{
						index_stream.seekg(-1, ios_base::cur); // 1�����߂�
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
						index_stream >> indexNormal;
						// ���_�f�[�^�̒ǉ�
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
						// �G�b�W�������p�̃f�[�^��ǉ�
						//if (smoothing)
						//{
						//	mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						//}
					}
				}
				// �C���f�b�N�X�f�[�^�̒ǉ�
				if (faceIndexCount >= 3) {
					// �l�p�`�|���S����4�_�ڂȂ̂ŁA
					// �l�p�`��0,1,2,3�̓� 2,3,0�ŎO�p�`���\�z����
					mesh->AddIndex(indexCountTex - 1);
					mesh->AddIndex(indexCountTex);
					mesh->AddIndex(indexCountTex - 3);
				}
				else
				{
					mesh->AddIndex(indexCountTex);
				}
				indexCountTex++;
				faceIndexCount++;
				////�C���f�b�N�X�f�[�^�̒ǉ�
				//indices.emplace_back((unsigned short)indices.size());
				////�E�����̏C��
				//if (countNum == 2)
				//{
				//	swap(indices[indices.size() - 1], indices[indices.size() - 2]);
				//}
				//else
				//{
				//	countNum++;
				//}
			}
		}
	}
	//�t�@�C�������
	file.close();

	// ���_�@���̕��ςɂ��G�b�W�̕�����
	//if (smoothing)
	//{
	//	mesh->CalculateSmoothedVertexNormals();
	//}
}

void Model::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	descHeapDesc.NumDescriptors = 1; // �V�F�[�_�[���\�[�X�r���[1��
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//����
	if (FAILED(result)) {
		assert(0);
	}

	// �f�X�N���v�^�T�C�Y���擾
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::CreateBuffers()
{

	//���b�V���̃N���G�C�g�o�b�t�@�Ɉڍs
	
	//HRESULT result = S_FALSE;
	//UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	//UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());


	//// ���_�o�b�t�@����
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&vertBuff));
	//if (FAILED(result)) {
	//	assert(0);
	//	return;
	//}

	//// �C���f�b�N�X�o�b�t�@����
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&indexBuff));
	//if (FAILED(result)) {
	//	assert(0);
	//	return;
	//}

	//// ���_�o�b�t�@�ւ̃f�[�^�]��
	//VertexPosNormalUv *vertMap = nullptr;
	//result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	//if (SUCCEEDED(result)) {
	//	std::copy(vertices.begin(), vertices.end(), vertMap);
	//	vertBuff->Unmap(0, nullptr);
	//}

	//// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	//unsigned short *indexMap = nullptr;
	//result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	//if (SUCCEEDED(result)) {

	//	// �S�C���f�b�N�X�ɑ΂���
	//	std::copy(indices.begin(), indices.end(), indexMap);

	//	indexBuff->Unmap(0, nullptr);
	//}

	//// ���_�o�b�t�@�r���[�̍쐬
	//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeVB;
	//vbView.StrideInBytes = sizeof(vertices[0]);

	//// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	//ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeIB;

	//�}�e���A���̃C�j�V�����C�Y�Ɉڍs
	//// �萔�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffB1));

	//�}�e���A���̃A�b�v�f�[�g�Ɉڍs
	// �萔�o�b�t�@�փf�[�^�]��
	//ConstBufferDataB1 *constMap1 = nullptr;
	//result = constBuffB1->Map(0, nullptr, (void **)&constMap1);
	//if (SUCCEEDED(result))
	//{
	//	constMap1->ambient = material.ambient;
	//	constMap1->diffuse = material.diffuse;
	//	constMap1->specular = material.specular;
	//	constMap1->alpha = material.alpha;
	//	constBuffB1->Unmap(0, nullptr);
	//}
}


void Model::AddMaterial(Material* material)
{
	// �R���e�i�ɓo�^
	materials.emplace(material->name, material);
}

void Model::LoadMaterial(const std::string &directoryPath, const std::string &filename)
{
	//�t�@�C���X�g���[��
	std::ifstream file;
	//�}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	//�t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	//1�s���ǂݍ���
	string line;
	while (getline(file, line))
	{
		//1�s���̕�������X�g���[���ɕϊ�
		std::istringstream line_stream(line);
		//���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		//�擪�^�u�����͖�������
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //�擪�̕������폜
		}

		//�擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl")
		{
			// ���Ƀ}�e���A���������
			if (material) {
				// �}�e���A�����R���e�i�ɓo�^
				AddMaterial(material);
			}

			// �V�����}�e���A���𐶐�
			material = Material::Create();
			// �}�e���A�����ǂݍ���
			line_stream >> material->name;
		}

		//�擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		//�擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		//�擪������Ks�Ȃ�f�B�t���[�Y�F
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		//�擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd")
		{
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material->textureFilename;

			// �t���p�X����t�@�C���������o��
			size_t pos1;
			pos1 = material->textureFilename.rfind('\\');
			if (pos1 != string::npos) {
				material->textureFilename = material->textureFilename.substr(pos1 + 1, material->textureFilename.size() - pos1 - 1);
			}

			pos1 = material->textureFilename.rfind('/');
			if (pos1 != string::npos) {
				material->textureFilename = material->textureFilename.substr(pos1 + 1, material->textureFilename.size() - pos1 - 1);
			}

			////�e�N�X�`���̃t�@�C�����ǂݍ���
			//line_stream >> material.textureFilename;
			////�e�N�X�`���ǂݍ���
			//LoadTexture(directoryPath, material.textureFilename);
		}
	}

	//�t�@�C�������
	file.close();

	if (material) {
		// �}�e���A����o�^
		AddMaterial(material);
	}
}

void Model::LoadTexture()
{
	int textureIndex = 0;
	string directoryPath = baseDirectory + name + "/";

	for (auto& m : materials) {
		Material* material = m.second;

		// �e�N�X�`������
		if (material->textureFilename.size() > 0) {
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);
			textureIndex++;
		}
		// �e�N�X�`���Ȃ�
		else {
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			// �}�e���A���Ƀe�N�X�`���ǂݍ���
			material->LoadTexture(baseDirectory, cpuDescHandleSRV, gpuDescHandleSRV);
			textureIndex++;
		}
	}


	//�}�e���A���N���X�Ɉڍs
	//HRESULT result = S_FALSE;

	//// WIC�e�N�X�`���̃��[�h
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};

	////�t�@�C���p�X������
	//string filepath = directoryPath + filename;

	////���j�R�[�h������ɕϊ�����
	//wchar_t wfilepath[128];
	//int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);


	//const Image *img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	//// ���\�[�X�ݒ�
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	//// �e�N�X�`���p�o�b�t�@�̐���
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
	//	nullptr,
	//	IID_PPV_ARGS(&texbuff));


	//// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	//result = texbuff->WriteToSubresource(
	//	0,
	//	nullptr, // �S�̈�փR�s�[
	//	img->pixels,    // ���f�[�^�A�h���X
	//	(UINT)img->rowPitch,  // 1���C���T�C�Y
	//	(UINT)img->slicePitch // 1���T�C�Y
	//);


	//// �V�F�[�_���\�[�X�r���[�쐬
	//cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	//gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	//D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	//srvDesc.Format = resDesc.Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	//srvDesc.Texture2D.MipLevels = 1;

	//device->CreateShaderResourceView(texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
	//	&srvDesc, //�e�N�X�`���ݒ���
	//	cpuDescHandleSRV
	//);
}

void Model::Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParamIndexMaterial)
{
	//// �f�X�N���v�^�q�[�v�̔z��
	//ID3D12DescriptorHeap *ppHeaps[] = { descHeap.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// �f�X�N���v�^�q�[�v�̔z��
	if (descHeap)
	{
		ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	// �S���b�V����`��
	for (auto& mesh : meshes)
	{
		mesh->Draw(cmdList);
	}
	

	//���b�V���̕`�揈���Ɉڍs
	////���_�o�b�t�@�̐ݒ�
	//cmdList->IASetVertexBuffers(0, 1, &vbView);
	////�C���f�b�N�X�o�b�t�@�̐ݒ�
	//cmdList->IASetIndexBuffer(&ibView);
	////�萔�o�b�t�@�r���[���Z�b�g (�}�e���A��)
	//cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());
	//if (material.textureFilename.size() > 0)
	//{
	//	//�V�F�[�_���\�[�X�r���[���Z�b�g
	//	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	//}
	//
	//// �`��R�}���h
	//cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
