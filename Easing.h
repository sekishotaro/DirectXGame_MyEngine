#pragma once
#include "MyMath.h"

class Easing
{
public:

	/// <summary>
	/// イージング
	/// </summary>
	/// <param name="t">時間</param>
	/// <param name="b">開始位置</param>
	/// <param name="c">終了位置との差</param>
	/// <param name="d">合計時間</param>
	/// <returns>座標</returns>
	static double easeIn(const double& t, const double& b, const double& c, const double& d);

	static double easeOut(const double& t, const double& b, const double& c, const double& d);

	static double easeInCubic(const double& x);
	
	static float easeOutCubic(const float& x);
};

