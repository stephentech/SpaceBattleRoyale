#include "stdafx.h"
#include "CommonConversion.h"

float DegreeToRadian(float aDegrees)
{
	return aDegrees * 3.142f / 180.0f;
}

float RadianToDegree(float aRadians)
{
	return aRadians * 180.0f / 3.142f;
}

int RandomRange(int min, int max)
{
	return (int)(((double)rand() / RAND_MAX) * (max - min) + min);
}
