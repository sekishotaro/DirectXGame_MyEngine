#pragma once

#include <DirectXMath.h>

class Object3d;
class BaseCollider;

/// <summary>
/// クエリ―による情報を得る為の構造体
/// </summary>
struct QueryHit
{
	//衝突相手のオブジェクト
	Object3d* object = nullptr;
	//衝突相手のコライダー
	BaseCollider* collider = nullptr;
	//衝突点
	DirectX::XMVECTOR inter;
	//排斥ベクトル
	DirectX::XMVECTOR reject;
};

/// <summary>
/// クエリ―で交差を検出した時の動作を規定するクラス
/// </summary>
class QueryCallback
{
public:
	QueryCallback() = default;
	virtual ~QueryCallback() = default;

	/// <summary>
	/// 交差時コールバック
	/// </summary>
	/// <param name="info">交差点</param>
	/// <returns>クエリ―を続けるならtrue、打ち切るならfalseを返す</returns>
	virtual bool OnQueryHit(const QueryHit& info) = 0;

};

