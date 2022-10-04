#include "Mesh.h"
#include <d3dcompiler.h>
#include <cassert>

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* Mesh::device = nullptr;

void Mesh::StaticInitialize(ID3D12Device* device)
{
	// 再初期化チェック
	assert(!Mesh::device);

	Mesh::device = device;

	// マテリアルの静的初期化
	Material::StaticInitialize(device);
}

void Mesh::SetName(const std::string& name)
{
	this->name = name;
}

void Mesh::AddVertex(const VertexPosNormalUv& vertex)
{
	vertices.emplace_back(vertex);
}

void Mesh::AddIndex(unsigned short index)
{
	indices.emplace_back(index);
}

void Mesh::SwapIndex()
{
	std::swap(indices[indices.size() - 1], indices[indices.size() - 2]);
}

void Mesh::SetMaterial(Material* material)
{
	this->material = material;
}

void Mesh::CreateBuffers()
{
	HRESULT result = S_FALSE;
	
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	
	if (FAILED(result))
	{
		assert(0);
		return;
	}

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result))
	{
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	
	if (FAILED(result))
	{
		assert(0);
		return;
	}
	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result))
	{
		// 全インデックスに対して
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}
	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}

void Mesh::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファの設定
	cmdList->IASetIndexBuffer(&ibView);
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(2, material->GetGpuHandle());
	// マテリアルの定数バッファをセット
	ID3D12Resource* constBuff = material->GetConstantBuffer();
	cmdList->SetGraphicsRootConstantBufferView(1, constBuff->GetGPUVirtualAddress());

	//定数バッファビューをセット (マテリアル)
	//cmdList->SetGraphicsRootConstantBufferView(rootParamIndexMaterial, constBuffB1->GetGPUVirtualAddress());

	//if (material.textureFilename.size() > 0)
	//{
	//	//シェーダリソースビューをセット
	//	cmdList->SetGraphicsRootDescriptorTable(2, gpuDescHandleSRV);
	//}

	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
