#pragma once
#include "Vector3D.h"
class Camera
{
public:
	Camera();
	Camera(const Camera&);

	void setCameraAngles(double, double);


	Vector3D dir, orient1, orient2;
};

