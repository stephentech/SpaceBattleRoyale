
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <math.h>

class CMatrix2
{
public:
	float m11, m12;
	float m21, m22;

	static CMatrix2 generateRotationMatrix(float fRadians)
	{
		CMatrix2 mat;
		mat.m11 = mat.m22 = cos(fRadians);
		mat.m12 = sin(fRadians);
		mat.m21 = -mat.m12;
		return mat;
	}
};

#endif // __MATRIX_H__