#pragma once
#include <cmath>
class Vector3D
{
public:
	Vector3D();
	Vector3D(double x, double y, double z);

	double dotMul(Vector3D vector);
	Vector3D crossMul(Vector3D vector);

	void normalize();
	Vector3D getNormolized();
	double getNorm();
	double getNormSqr();

	Vector3D operator+(Vector3D vector);
	Vector3D operator-(Vector3D vector);
	Vector3D operator/(double number);
	Vector3D operator*(double number);

	double x, y, z;

};

