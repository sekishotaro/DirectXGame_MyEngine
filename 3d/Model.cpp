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
/// 静的メンバ変数の実体
/// </summary>
const std::string Model::baseDirectory = "Resources/";
ID3D12Device* Model::device = nullptr;
UINT Model::descriptorHandleIncrementSize = 0;

Model *Model::LoadFromOBJ(const std::string &modelname)
{
	//新たなModel型のインスタンスのメモリを確保
	Model* model = new Model();
	
	//読み込み
	model->Initialize(modelname);

	return model;
}

void Model::SetDevice(ID3D12Device* device)
{
	// 再初期化チェック
	assert(!Model::device);

	Model::device = device;

	// メッシュの静的初期化
	Mesh::StaticInitialize(device);
}

void Model::Initialize(const std::string& modelname)
{
	LoadFormOBJInternal(modelname);

	// メッシュのマテリアルチェック
	for (auto& m : meshes) {
		// マテリアルの割り当てがない
		if (m->GetMaterial() == nullptr) {
			if (defaultMaterial == nullptr) {
				// デフォルトマテリアルを生成
				defaultMaterial = Material::Create();
				defaultMaterial->name = "no material";
				materials.emplace(defaultMaterial->name, defaultMaterial);
			}
			// デフォルトマテリアルをセット
			m->SetMaterial(defaultMaterial);
		}
	}

	//バッファ生成
	//model->CreateBuffers();
	// メッシュのバッファ生成
	for (auto& m : meshes) {
		m->CreateBuffers();
	}

	// マテリアルの数値を定数バッファに反映
	for (auto& m : materials) {
		m.second->Update();
	}

	//デスクリプタヒープの生成
	InitializeDescriptorHeap();

	// テクスチャの読み込み
	LoadTexture();
}

void Model::LoadFormOBJInternal(const std::string &modelname)
{
	//.objファイルを開く
	const string filename = modelname + ".obj";                  //"triangle_mat.obj"
	const string directoryPath = "Resources/" + modelname + "/"; //"Resources/triangle_mat/"
	
	//ファイルストリーム
	std::ifstream file;
	file.open(directoryPath + filename);                         //"Resources/triangle_mat/triangle_mat.obj"
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	name = modelname;

	// メッシュ生成
	meshes.emplace_back(new Mesh);
	Mesh* mesh = meshes.back();
	int indexCountTex = 0;
	int indexCountNoTex = 0;

	vector<XMFLOAT3> positions;    //頂点座標
	vector<XMFLOAT3> normals;      //法線ベクトル
	vector<XMFLOAT2> texcoords;    //テクスチャUV
	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペースゥ切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib")
		{
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}
		// 先頭文字列がgならグループの開始
		if (key == "g") {

			// カレントメッシュの情報が揃っているなら
			if (mesh->GetName().size() > 0 && mesh->GetVertexCount() > 0) {
				// 頂点法線の平均によるエッジの平滑化
				//if (smoothing)
				//{
				//	mesh->CalculateSmoothedVertexNormals();
				//}
				// 次のメッシュ生成
				meshes.emplace_back(new Mesh);
				mesh = meshes.back();
				indexCountTex = 0;
			}

			// グループ名読み込み
			string groupName;
			line_stream >> groupName;

			// メッシュに名前をセット
			mesh->SetName(groupName);
		}

		//先頭文字がvなら頂点座標
		if (key == "v")
		{
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt")
		{
			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//テクスチャ座標データに追加
			texcoords.emplace_back(texcoord);
		}
		//先頭文字列がvnなら法線ベクトル
		if (key == "vn")
		{
			//X,Y,Z成分読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//法線ベクトルデータに追加
			normals.emplace_back(normal);
		}
		// 先頭文字列がusemtlならマテリアルを割り当てる
		if (key == "usemtl")
		{
			if (mesh->GetMaterial() == nullptr) {
				// マテリアルの名読み込み
				string materialName;
				line_stream >> materialName;

				// マテリアル名で検索し、マテリアルを割り当てる
				auto itr = materials.find(materialName);
				if (itr != materials.end()) {
					mesh->SetMaterial(itr->second);
				}
			}
		}

		//先頭文字列がfならポリゴン (三角形)
		if (key == "f")
		{
			int faceIndexCount = 0;
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			int countNum = 0;
			while (getline(line_stream, index_string, ' '))
			{
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				//頂点番号
				index_stream >> indexPosition;

				Material* material = mesh->GetMaterial();
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				//マテリアル、テクスチャがある場合
				if (material && material->textureFilename.size() > 0)
				{
					index_stream >> indexTexcoord;
					index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
					index_stream >> indexNormal;
					//頂点データの追加
					Mesh::VertexPosNormalUv vertex{};
					vertex.pos = positions[indexPosition - 1];
					vertex.normal = normals[indexNormal - 1];
					vertex.uv = texcoords[indexTexcoord - 1];
					mesh->AddVertex(vertex);
					// エッジ平滑化用のデータを追加
					//if (smoothing)
					//{
					//	mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
					//}
				}
				else
				{
					char c;
					index_stream >> c;
					// スラッシュ2連続の場合、頂点番号のみ
					if (c == '/')
					{
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = { 0, 0, 1 };
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
					}
					else
					{
						index_stream.seekg(-1, ios_base::cur); // 1文字戻る
						index_stream >> indexTexcoord;
						index_stream.seekg(1, ios_base::cur); // スラッシュを飛ばす
						index_stream >> indexNormal;
						// 頂点データの追加
						Mesh::VertexPosNormalUv vertex{};
						vertex.pos = positions[indexPosition - 1];
						vertex.normal = normals[indexNormal - 1];
						vertex.uv = { 0, 0 };
						mesh->AddVertex(vertex);
						// エッジ平滑化用のデータを追加
						//if (smoothing)
						//{
						//	mesh->AddSmoothData(indexPosition, (unsigned short)mesh->GetVertexCount() - 1);
						//}
					}
				}
				// インデックスデータの追加
				if (faceIndexCount >= 3) {
					// 四角形ポリゴンの4点目なので、
					// 四角形の0,1,2,3の内 2,3,0で三角形を構築する
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
				////インデックスデータの追加
				//indices.emplace_back((unsigned short)indices.size());
				////右軸回りの修正
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
	//ファイルを閉じる
	file.close();

	// 頂点法線の平均によるエッジの平滑化
	//if (smoothing)
	//{
	//	mesh->CalculateSmoothedVertexNormals();
	//}
}

void Model::InitializeDescriptorHeap()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成	
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダから見えるように
	descHeapDesc.NumDescriptors = 1; // シェーダーリソースビュー1つ
	result = device->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&descHeap));//生成
	if (FAILED(result)) {
		assert(0);
	}

	// デスクリプタサイズを取得
	descriptorHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void Model::CreateBuffers()
{

	//メッシュのクリエイトバッファに移行
	
	//HRESULT result = S_FALSE;
	//UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	//UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());


	//// 頂点バッファ生成
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

	//// インデックスバッファ生成
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

	//// 頂点バッファへのデータ転送
	//VertexPosNormalUv *vertMap = nullptr;
	//result = vertBuff->Map(0, nullptr, (void **)&vertMap);
	//if (SUCCEEDED(result)) {
	//	std::copy(vertices.begin(), vertices.end(), vertMap);
	//	vertBuff->Unmap(0, nullptr);
	//}

	//// インデックスバッファへのデータ転送
	//unsigned short *indexMap = nullptr;
	//result = indexBuff->Map(0, nullptr, (void **)&indexMap);
	//if (SUCCEEDED(result)) {

	//	// 全インデックスに対して
	//	std::copy(indices.begin(), indices.end(), indexMap);

	//	indexBuff->Unmap(0, nullptr);
	//}

	//// 頂点バッファビューの作成
	//vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	//vbView.SizeInBytes = sizeVB;
	//vbView.StrideInBytes = sizeof(vertices[0]);

	//// インデックスバッファビューの作成
	//ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	//ibView.Format = DXGI_FORMAT_R16_UINT;
	//ibView.SizeInBytes = sizeIB;

	//マテリアルのイニシャライズに移行
	//// 定数バッファの生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
	//	D3D12_HEAP_FLAG_NONE,
	//	&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
	//	D3D12_RESOURCE_STATE_GENERIC_READ,
	//	nullptr,
	//	IID_PPV_ARGS(&constBuffB1));

	//マテリアルのアップデートに移行
	// 定数バッファへデータ転送
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
	// コンテナに登録
	materials.emplace(material->name, material);
}

void Model::LoadMaterial(const std::string &directoryPath, const std::string &filename)
{
	//ファイルストリーム
	std::ifstream file;
	//マテリアルファイルを開く
	file.open(directoryPath + filename);
	//ファイルオープン失敗をチェック
	if (file.fail())
	{
		assert(0);
	}

	Material* material = nullptr;

	//1行ずつ読み込む
	string line;
	while (getline(file, line))
	{
		//1行分の文字列をストリームに変換
		std::istringstream line_stream(line);
		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭タブ文字は無視する
		if (key[0] == '\t')
		{
			key.erase(key.begin()); //先頭の文字を削除
		}

		//先頭文字列がnewmtlならマテリアル名
		if (key == "newmtl")
		{
			// 既にマテリアルがあれば
			if (material) {
				// マテリアルをコンテナに登録
				AddMaterial(material);
			}

			// 新しいマテリアルを生成
			material = Material::Create();
			// マテリアル名読み込み
			line_stream >> material->name;
		}

		//先頭文字列がKaならアンビエント色
		if (key == "Ka")
		{
			line_stream >> material->ambient.x;
			line_stream >> material->ambient.y;
			line_stream >> material->ambient.z;
		}

		//先頭文字列がKdならディフューズ色
		if (key == "Kd")
		{
			line_stream >> material->diffuse.x;
			line_stream >> material->diffuse.y;
			line_stream >> material->diffuse.z;
		}

		//先頭文字列がKsならディフューズ色
		if (key == "Ks")
		{
			line_stream >> material->specular.x;
			line_stream >> material->specular.y;
			line_stream >> material->specular.z;
		}

		//先頭文字列がmap_Kdならテクスチャファイル名
		if (key == "map_Kd")
		{
			// テクスチャのファイル名読み込み
			line_stream >> material->textureFilename;

			// フルパスからファイル名を取り出す
			size_t pos1;
			pos1 = material->textureFilename.rfind('\\');
			if (pos1 != string::npos) {
				material->textureFilename = material->textureFilename.substr(pos1 + 1, material->textureFilename.size() - pos1 - 1);
			}

			pos1 = material->textureFilename.rfind('/');
			if (pos1 != string::npos) {
				material->textureFilename = material->textureFilename.substr(pos1 + 1, material->textureFilename.size() - pos1 - 1);
			}

			////テクスチャのファイル名読み込み
			//line_stream >> material.textureFilename;
			////テクスチャ読み込み
			//LoadTexture(directoryPath, material.textureFilename);
		}
	}

	//ファイルを閉じる
	file.close();

	if (material) {
		// マテリアルを登録
		AddMaterial(material);
	}
}

void Model::LoadTexture()
{
	int textureIndex = 0;
	string directoryPath = baseDirectory + name + "/";

	for (auto& m : materials) {
		Material* material = m.second;

		// テクスチャあり
		if (material->textureFilename.size() > 0) {
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(directoryPath, cpuDescHandleSRV, gpuDescHandleSRV);
			textureIndex++;
		}
		// テクスチャなし
		else {
			CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), textureIndex, descriptorHandleIncrementSize);
			// マテリアルにテクスチャ読み込み
			material->LoadTexture(baseDirectory, cpuDescHandleSRV, gpuDescHandleSRV);
			textureIndex++;
		}
	}


	//マテリアルクラスに移行
	//HRESULT result = S_FALSE;

	//// WICテクスチャのロード
	//TexMetadata metadata{};
	//ScratchImage scratchImg{};

	////ファイルパスを結合
	//string filepath = directoryPath + filename;

	////ユニコード文字列に変換する
	//wchar_t wfilepath[128];
	//int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	//result = LoadFromWICFile(wfilepath, WIC_FLAGS_NONE, &metadata, scratchImg);


	//const Image *img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	//// リソース設定
	//CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
	//	metadata.format,
	//	metadata.width,
	//	(UINT)metadata.height,
	//	(UINT16)metadata.arraySize,
	//	(UINT16)metadata.mipLevels
	//);

	//// テクスチャ用バッファの生成
	//result = device->CreateCommittedResource(
	//	&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
	//	D3D12_HEAP_FLAG_NONE,
	//	&texresDesc,
	//	D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
	//	nullptr,
	//	IID_PPV_ARGS(&texbuff));


	//// テクスチャバッファにデータ転送
	//result = texbuff->WriteToSubresource(
	//	0,
	//	nullptr, // 全領域へコピー
	//	img->pixels,    // 元データアドレス
	//	(UINT)img->rowPitch,  // 1ラインサイズ
	//	(UINT)img->slicePitch // 1枚サイズ
	//);


	//// シェーダリソースビュー作成
	//cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);
	//gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descriptorHandleIncrementSize);

	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	//D3D12_RESOURCE_DESC resDesc = texbuff->GetDesc();

	//srvDesc.Format = resDesc.Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	//srvDesc.Texture2D.MipLevels = 1;

	//device->CreateShaderResourceView(texbuff.Get(), //ビューと関連付けるバッファ
	//	&srvDesc, //テクスチャ設定情報
	//	cpuDescHandleSRV
	//);
}

void Model::Draw(ID3D12GraphicsCommandList *cmdList, UINT rootParamIndexMaterial)
{
	//// デスクリプタヒープの配列
	//ID3D12DescriptorHeap *ppHeaps[] = { descHeap.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// デスクリプタヒープの配列
	if (descHeap)
	{
		ID3D12DescriptorHeap* ppHeaps[] = { descHeap.Get() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	}

	// 全メッシュを描画
	for (auto& mesh : meshes)
	{
		mesh->Draw(cmdList);
	}
	

	//メッシュの描画処理に移行
	////頂点バッファの設定
	//cmdList->IASetVertexBuffers(0, 1, &vbView);
	////インデックスバッファの設定
	//cmdList->IASetIndexBuffer(&ibView);
	////定数バッファビューをセット (マテリアル)
	//cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());
	//if (material.textureFilename.size() > 0)
	//{
	//	//シェーダリソースビューをセット
	//	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	//}
	//
	//// 描画コマンド
	//cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
