#pragma once
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

namespace Random
{
	static void Seed()
	{
		srand(static_cast <unsigned> (time(0)));
	}

	static float RandomFloatInRange(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	static int RandomIntInRange(int min, int max)
	{
		return rand() % (max - min) + min;
	}
}