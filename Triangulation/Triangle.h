#pragma once
#include "Vector3D.h"
#include "Camera.h"
#include "framework.h"

class Triangle
{
public:
	Triangle();
	Triangle(Vector3D, Vector3D, Vector3D);
	Triangle(const Triangle&);

	double distanceToCamera(Camera cam);
	Triangle projectTriangle(Camera cam);

	void calcTriangleNorm();
	POINT* getPoints();

	Vector3D vertexs[3];
	Vector3D norm;
	HBRUSH brush;
};

