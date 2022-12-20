#pragma once
class ClockTime
{
public:

	static void ClockUpdate();

	static double GetSec() { return sec; }

	static bool GetAddSecFlag() { return addSecFlag; }

	static double sec;
	static long cpu_time;

	static int oldSec;

	static bool addSecFlag;
};

