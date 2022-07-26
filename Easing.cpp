#include "Easing.h"

double Easing::easeIn(const double& t, const double& b, const double& c, const double& d)
{
	double x = t / d;
	double v = easeInCubic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::easeOut(const double& t, const double& b, const double& c, const double& d)
{
	double x = t / d;
	double v = easeOutCubic(x);
	double ret = c * v + b;
	return ret;
}

double Easing::easeInCubic(const double& x)
{
	return x * x * x;
}

float Easing::easeOutCubic(const float& x)
{
	return 1 - pow(1 - x, 3);
}

