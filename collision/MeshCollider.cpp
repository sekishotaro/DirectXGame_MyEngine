﻿#include "MeshCollider.h"
#include "Collision.h"

using namespace DirectX;

void MeshCollider::ConstructTriangles(Model* model)
{
	//三角形リストをクリア
	triangles.clear();
	//モデルの持つメッシュリストを取得
	const std::vector<Mesh*>& meshes = model->GetMeshes();
	//現在のメッシュの開始三角形番号を入れておく変数(0で初期化)
	int start = 0;
	//全メッシュについて順に処理する
	std::vector<Mesh*>::const_iterator it = meshes.cbegin();
	for (; it != meshes.cend(); ++it)
	{
		Mesh* mesh = *it;
		const std::vector<Mesh::VertexPosNormalUv>& vertices = mesh->GetVertices();
		const std::vector<unsigned short>& indices = mesh->GetIndices();

		//インデックスは、三角形の数×3個あるので、
		//そこからメッシュ内の三角形の数を逆算する
		size_t triangleNum = indices.size() / 3;
		//現在のメッシュの三角形の数だけ、三角形リストにスペースを追加する
		triangles.resize(triangles.size() + triangleNum);
		//全三角形について順に処理する
		for (int i = 0; i < triangleNum; i++)
		{
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];
			// 三角形の3頂点の座標を代入
			tri.p0 = { vertices[idx0].pos.x, vertices[idx0].pos.y, vertices[idx0].pos.z, 1 };
			tri.p1 = { vertices[idx1].pos.x, vertices[idx1].pos.y, vertices[idx1].pos.z, 1 };
			tri.p2 = { vertices[idx2].pos.x, vertices[idx2].pos.y, vertices[idx2].pos.z, 1 };
			//3頂点から法線を計算
			tri.ComputeNormal();
		}

		//次のメッシュは、今までの三角形番号の次から使う。
		start += (int)triangleNum;
	}
}

void MeshCollider::Update()
{
	//ワールド行列の逆行列を計算
	invMatWorld = XMMatrixInverse(nullptr, GetObject3d()->GetMatWorld());
}

bool MeshCollider::CheckCollisionSphere(const Sphere& sphere, DirectX::XMVECTOR* inter)
{
	//オブジェクトのローカル座標系での球を得る (半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = XMVector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= XMVector3Length(invMatWorld.r[0]).m128_f32[0];

	//ローカル座標系で交差をチェック
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;

		//球と三角形の当たり判定
		if (Collision::CheckSphere2Triangle(localSphere, triangle, inter))
		{
			if (inter)
			{
				const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
				//ワールド座標系での交点を得る
				*inter = XMVector3Transform(*inter, matWorld);
			}
			return true;
		}
	}
	return false;
}

bool MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, DirectX::XMVECTOR* inter)
{
	Ray localRay;
	localRay.start = XMVector3Transform(ray.start, invMatWorld);
	localRay.dir = XMVector3TransformNormal(ray.dir, invMatWorld);

	//ローカル座標系で交差をチェック
	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it)
	{
		const Triangle& triangle = *it;

		XMVECTOR tempInter;
		//レイと三角形の当たり判定
		if (Collision::CheckRay2Triangle(localRay, triangle, nullptr, &tempInter))
		{
			const XMMATRIX& matWorld = GetObject3d()->GetMatWorld();
			//ワールド座標系での交点を得る
			tempInter = XMVector3Transform(tempInter, matWorld);

			if (distance)
			{
				//交点とレイ始点の距離を計算
				XMVECTOR sub = tempInter - ray.start;
				*distance = XMVector3Dot(sub, ray.dir).m128_f32[0];
			}

			if (inter)
			{
				*inter = tempInter;
			}

			return true;
		}
	}
	return false;
}
