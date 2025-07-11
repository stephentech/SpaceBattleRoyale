
#ifndef __COMMON_CONVERSIONS_H__
#define __COMMON_CONVERSIONS_H__

#ifndef NULL
	#define NULL 0
#endif

typedef unsigned int uint32;
typedef unsigned long ulong;

float DegreeToRadian(float fDegrees);
float RadianToDegree(float fRadians);

int RandomRange(int min, int max);

template<class T>
void clamp( T& val, T min, T max)
{
	if(val < min)
		val = min;
	if(val > max)
		val = max;
}

#define SAFE_CALL(x) if(x)x->

#endif // __COMMON_CONVERSIONS_H__