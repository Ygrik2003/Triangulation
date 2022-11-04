#include "Camera.h"

Camera::Camera()
{
	dir		= Vector3D(0, 0, -1);
	orient1 = Vector3D(0, 1, 0);
	orient2 = Vector3D(-1, 0, 0);
}

Camera::Camera(const Camera& camera)
{
	this->dir = camera.dir;
	this->orient1 = camera.orient1;
	this->orient2 = camera.orient2;
}

void Camera::setCameraAngles(double theta, double phi)
{
	dir		= Vector3D(-sin(theta) * cos(phi), -sin(theta) * sin(phi), -cos(theta));
	orient1 = Vector3D(-sin(phi), cos(phi), 0);
	orient2 = Vector3D(-cos(theta) * cos(phi), -cos(theta) * sin(phi), sin(theta));
}
