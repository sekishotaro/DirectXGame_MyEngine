#pragma once
#include "Object3d.h"
class TouchableObject : public Object3d
{
public:

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(Model* model = nullptr);

	bool Initialize(Model* model);
};

