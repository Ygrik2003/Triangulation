#include "Vector3D.h"

Vector3D::Vector3D()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3D::Vector3D(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

double Vector3D::dotMul(Vector3D vector)
{

	return this->x * vector.x + this->y * vector.y + this->z * vector.z;
}

Vector3D Vector3D::crossMul(Vector3D vector)
{
	return Vector3D(this->y * vector.z - this->z * vector.y, this->z * vector.x - this->x * vector.z, this->x * vector.y - this->y * vector.x);
}

void Vector3D::normalize()
{
	double norm = getNorm();

	this->x /= norm;
	this->y /= norm;
	this->z /= norm;
}

Vector3D Vector3D::getNormolized()
{
	double norm = getNorm();
	return Vector3D(this->x / norm, this->y / norm, this->z / norm);
}

double Vector3D::getNorm()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

double Vector3D::getNormSqr()
{
	return pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2);
}

Vector3D Vector3D::operator+(Vector3D vector)
{
	return Vector3D(this->x + vector.x, this->y + vector.y, this->z + vector.z);
}

Vector3D Vector3D::operator-(Vector3D vector)
{
	return Vector3D(this->x - vector.x, this->y - vector.y, this->z - vector.z);
}

Vector3D Vector3D::operator/(double number)
{
	return Vector3D(this->x / number, this->y / number, this->z / number);
}

Vector3D Vector3D::operator*(double number)
{
	return Vector3D(this->x * number, this->y * number, this->z * number);
}
