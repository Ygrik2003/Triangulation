# Triangulation
Triangulation on Win32api
In this project realized 2 methods for triangulation sphere and pyramid
```cpp
void paintSphere();
void paintPyramid();
```
For sphere we can setting radius
```cpp
const double R = 1;
```
For pyramid we can setting interval for y and z, and step dy and dz
```cpp
const double yStart = -1, yEnd = 1, dy = 0.2;
const double zStart = -1, zEnd = 1, dz = 0.2;
```

So also we can set diraction point of view for camera and rotate this camera.
```cpp
camera.setCameraAngles(0, PI / 2);
```
