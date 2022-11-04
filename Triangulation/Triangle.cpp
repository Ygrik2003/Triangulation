#include "Triangle.h"

Triangle::Triangle()
{
	this->vertexs[0] = Vector3D(0, 0, 0);
	this->vertexs[1] = Vector3D(0, 0, 0);
	this->vertexs[2] = Vector3D(0, 0, 0);
}

Triangle::Triangle(Vector3D a, Vector3D b, Vector3D c)
{
	this->vertexs[0] = a;
	this->vertexs[1] = b;
	this->vertexs[2] = c;
}

Triangle::Triangle(const Triangle &triangle)
{
	for (int i = 0; i < 3; i++)
		this->vertexs[i] = triangle.vertexs[i];
}

double Triangle::distanceToCamera(Camera cam)
{
	Vector3D centerTriangle = (this->vertexs[0] + this->vertexs[1] + this->vertexs[2]) / 3;
	return centerTriangle.dotMul(cam.dir);
}

Triangle Triangle::projectTriangle(Camera cam)
{
	Triangle triangle;
	double distance;
	Vector3D projectVector;

	for (int i = 0; i < 3; i++)
	{
	distance = this->vertexs[i].dotMul(cam.dir);
	projectVector = vertexs[i] - cam.dir * distance;

	triangle.vertexs[i] = Vector3D(projectVector.dotMul(cam.orient1), projectVector.dotMul(cam.orient2), 0);

	}
	
	return triangle;
}

void Triangle::calcTriangleNorm()
{
	Vector3D firstVector, secondVector;

	firstVector = this->vertexs[1] - this->vertexs[0];
	secondVector = this->vertexs[2] - this->vertexs[0];

	this->norm = firstVector.crossMul(secondVector).getNormolized();
}

POINT* Triangle::getPoints()
{
	POINT points[] = { {vertexs[0].x, vertexs[0].y}, {vertexs[1].x, vertexs[1].y}, {vertexs[2].x, vertexs[2].y} };

	return points;
}
