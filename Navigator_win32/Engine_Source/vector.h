
#ifndef __POINT_H__
#define __POINT_H__

#include <math.h>
#include "Matrix.h"

class CVector
{
public:
	CVector()
	{
	}

	CVector(float _x, float _y)
	: x(_x)
	, y(_y)
	{
	}

	CVector& operator=(const CVector& pt)      
	{
		x = pt.x;
		y = pt.y;
		return *this;
	}

	CVector operator+(const CVector& pt) const
	{
		return CVector(x + pt.x, y + pt.y);
	}

	CVector operator-(const CVector& pt) const
	{
		return CVector(x - pt.x, y - pt.y);
	}

	CVector operator*(float f) const
	{
		return CVector(x * f, y * f);
	}

	CVector operator*(CMatrix2 aMat) const
	{
		float tempX = x * aMat.m11 + y * aMat.m21;
		float tempY = x * aMat.m12 + y * aMat.m22;

		return CVector(tempX, tempY);
	}

	CVector operator/(float f) const
	{
		return CVector(x / f, y / f);
	}


	bool operator==(CVector pt) const
	{
		return (x==pt.x && y==pt.y);
	}

	bool operator!=(CVector pt) const
	{
		return !(*this == pt);
	}

	void Normalise()
	{
		float f = GetLength();
		if(f != 0)
		{
			x /= f;
			y /= f;
		}
		else
		{
			x = y = 0;
		}
	}

	float DotProduct(CVector pt) const
	{
		return x * pt.x + y * pt.y;
	}

	float GetLength() const
	{
		if(x == 0.0f && y == 0.0f)
			return 0;
		return sqrt((float)(x * x + y * y));
	}

	CVector RotateBy(float aAngleInRadians)
	{
		*this = *this * CMatrix2::generateRotationMatrix(aAngleInRadians);
		return *this;
	}


public:
	float x;
	float y;
};

#endif // __POINT_H__