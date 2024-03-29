#pragma once

#include "CollisionTypes.h"
#include "Object3d.h"
#include "FbxObject3d.h"
#include "CollisionInfo.h"

/// <summary>
/// コライダー基底クラス
/// </summary>
class BaseCollider
{
public:
	friend class CollisionManager;

public:
	BaseCollider() = default;
	//仮想デストラクタ
	virtual ~BaseCollider() = default;

	inline void SetObject(Object3d* object) { this->object3d = object; }
	inline void SetFbxObject(FbxObject3d* fbxObject) { this->fbxObject = fbxObject; }

	inline Object3d* GetObject3d() { return object3d; }
	inline FbxObject3d* GetFbxObject3d() { return fbxObject; }

	inline void SetAttribute(unsigned short attribute)
	{
		this->attribute = attribute;
	}

	inline void AddAttribute(unsigned short attribute)
	{
		this->attribute |= attribute;
	}

	inline void RemoveAttribute(unsigned short attribute)
	{
		this->attribute &= !attribute;
	}

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;
	virtual void UpdateF() = 0;

	//形状タイプ取得
	inline CollisionShapeType GetShapeType() { return shapeType; }

	inline void OnCollision(const CollisionInfo& info) { object3d->OnCollision(info); }
	inline void OnCollisionF(const CollisionInfo& info) { fbxObject->OnCollision(info); }

protected:
	Object3d* object3d = nullptr;
	FbxObject3d* fbxObject = nullptr;
	//形状タイプ
	CollisionShapeType shapeType = SHAPE_UNKNOWN;
	unsigned short attribute = 0b1111111111111111;
};

