#pragma once
#include "MyMath.h"

class Easing
{
public:

	/// <summary>
	/// �C�[�W���O
	/// </summary>
	/// <param name="t">����</param>
	/// <param name="b">�J�n�ʒu</param>
	/// <param name="c">�I���ʒu�Ƃ̍�</param>
	/// <param name="d">���v����</param>
	/// <returns>���W</returns>
	static double easeIn(const double& t, const double& b, const double& c, const double& d);

	static double easeOut(const double& t, const double& b, const double& c, const double& d);

	static double easeInCubic(const double& x);
	
	static float easeOutCubic(const float& x);
};

