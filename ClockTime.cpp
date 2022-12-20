#include "ClockTime.h"
#include <time.h>

double ClockTime::sec = 0;
long ClockTime::cpu_time = 0;
int ClockTime::oldSec = 0;
bool ClockTime::addSecFlag = false;

void ClockTime::ClockUpdate()
{
	cpu_time = clock();

	oldSec = (int)sec;

	sec = (double)cpu_time / CLOCKS_PER_SEC;

	int secA = (int)sec;
	if (oldSec != secA)
	{
		addSecFlag = true;
	}
	else
	{
		addSecFlag = false;
	}
}
