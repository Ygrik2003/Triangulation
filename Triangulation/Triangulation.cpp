// TriangulationMy.cpp : Определяет точку входа для приложения.
//
#include "Triangulation.h"
#include "Camera.h"
#include "Triangle.h"

#define MAX_LOADSTRING 100
#define PI acos(-1)
#define IDT_TIMER 100

const double R = 1, dPhi = PI / 10, dTheta = PI / 10;

const double xStart = -2, xEnd = 2, dx = 0.2;
const double yStart = -1, yEnd = 1, dy = 0.2;
const double zStart = -1, zEnd = 1, dz = 0.2;


double phiCamera = 0, thetaCamera = PI / 3;

int MaxX, MaxY, offsetX, offsetY, scaleX, scaleY;
RECT rectWindow;
Triangle* triangles;
int index = 0;


Camera camera;

HBRUSH brushWhite = CreateSolidBrush(RGB(255, 255, 255));

HPEN penRed = CreatePen(PS_SOLID, 3, RGB(255, 255, 0));
HPEN penGreen = CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
HPEN penBlue = CreatePen(PS_SOLID, 3, RGB(0, 0, 255));

const double FPS = 30;

POINT* vectorToPoint(Triangle triangle);

void generate(HDC);
Vector3D getSphere(double, double);
void paintSphere();

Vector3D getPyramid(double phi, double theta);
Vector3D getPyramidPositive(double y, double z);
Vector3D getPyramidNegative(double y, double z);
void paintPyramid();

int x2ix(double);
int y2iy(double);

void sortZ(Triangle*, int);

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    camera.setCameraAngles(0, PI / 2);

    //camera.setCameraAngles(PI / 4, PI / 3);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRIANGULATIONMY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRIANGULATIONMY));

    MSG msg;

    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRIANGULATIONMY));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TRIANGULATIONMY);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance;

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC memBit;
    static HBITMAP hBitmap;
    switch (message)
    {
    case WM_CREATE:
    {
        HDC hdc = GetDC(hWnd);
        memBit = CreateCompatibleDC(hdc);
        ReleaseDC(hWnd, hdc);
        SetTimer(hWnd, IDT_TIMER, 1000 / FPS, NULL);
    }

        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            generate(memBit);

            BitBlt(hdc, 0, 0, MaxX, MaxY, memBit, 0, 0, SRCCOPY);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
        phiCamera += PI / 2 / FPS;
        if (phiCamera > (2 * PI))
            phiCamera -= 2 * PI;
        camera.setCameraAngles(thetaCamera, phiCamera);
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE);
        break;
    case WM_SIZE:
        MaxX = LOWORD(lParam);
        MaxY = HIWORD(lParam);
        scaleX = 0.8 * (min(MaxX, MaxY) / 2) / (zEnd - zStart);
        scaleY = 0.8 * (min(MaxX, MaxY) / 2) / (yEnd - yStart);
        offsetX = MaxX / 2; offsetY = MaxY / 2;

        GetWindowRect(hWnd, &rectWindow);

        hBitmap = CreateCompatibleBitmap(GetDC(hWnd), MaxX, MaxY);
        SelectObject(memBit, hBitmap);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);;
}



int x2ix(double x)
{
    return offsetX + scaleX * x;
}

int y2iy(double y)
{
    return offsetY - scaleY * y;
}

void generate(HDC hdc)
{
    
    paintPyramid();
    //paintSphere();
    

    

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = MaxX;
    rect.bottom = MaxY;
    sortZ(triangles, index);
    FillRect(hdc, &rect, brushWhite);

    SelectObject(hdc, GetStockObject(NULL_PEN));
    for (int i = 0; i < index; i++)
    {
        SelectObject(hdc, triangles[i].brush);
        Polygon(hdc, vectorToPoint(triangles[i].projectTriangle(camera)), 3);

        DeleteObject(triangles[i].brush);
    }
    Triangle line;
    line.vertexs[0] = Vector3D(0, 0, 0);
    line.vertexs[1] = Vector3D(0, 0, 0);
    line.vertexs[2] = Vector3D(1, 0, 0);
    SelectObject(hdc, penRed);
    Polygon(hdc, vectorToPoint(line.projectTriangle(camera)), 3);
    line.vertexs[2] = Vector3D(0, 1, 0);
    SelectObject(hdc, penGreen);
    Polygon(hdc, vectorToPoint(line.projectTriangle(camera)), 3);
    line.vertexs[2] = Vector3D(0, 0, 1);
    SelectObject(hdc, penBlue);
    Polygon(hdc, vectorToPoint(line.projectTriangle(camera)), 3);
}


POINT* vectorToPoint(Triangle triangle)
{
    POINT points[] = { {x2ix(triangle.vertexs[0].x), y2iy(triangle.vertexs[0].y) }, {x2ix(triangle.vertexs[1].x), y2iy(triangle.vertexs[1].y) }, {x2ix(triangle.vertexs[2].x), y2iy(triangle.vertexs[2].y) } };

    return points;
}

Vector3D getSphere(double phi, double theta)
{
    return Vector3D(R * cos(phi) * sin(theta), R * sin(phi) * sin(theta), R * cos(theta));
}

Vector3D getPyramid(double phi, double theta)
{
    return Vector3D(abs(R * cos(theta) + R * sin(phi) * sin(theta)) + abs(R * cos(theta) - R * sin(phi) * sin(theta)), R * sin(phi) * sin(theta), R * cos(theta));
}

Vector3D getPyramidPositive(double y, double z)
{
    return Vector3D(abs(z + y) + abs(z - y), y, z);
}

Vector3D getPyramidNegative(double y, double z)
{
    return Vector3D(-abs(z + y) - abs(z - y), y, z);
}

void paintPyramid()
{
    if (triangles == NULL)
        triangles = new Triangle[2 * ((yEnd - yStart) / dy + 1) * ((zEnd - zStart) / dz + 1) * 2];

    index = 0;

    double y = yStart, z = zStart;

    for (int yCount = 0; yCount < (yEnd - yStart) / dy; yCount++)
    {
        z = zStart;
        for (int zCount = 0; zCount < (zEnd - zStart) / dz; zCount++)
        {
            if ((z + dz) * (y + dy) < 0)
            {
                triangles[index].vertexs[0] = getPyramidPositive(y, z);
                triangles[index].vertexs[1] = getPyramidPositive(y, z + dz);
                triangles[index].vertexs[2] = getPyramidPositive(y + dy, z);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;


                triangles[index].vertexs[0] = getPyramidPositive(y, z + dz);
                triangles[index].vertexs[1] = getPyramidPositive(y + dy, z + dz);
                triangles[index].vertexs[2] = getPyramidPositive(y + dy, z);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;
            }
            else
            {
                triangles[index].vertexs[0] = getPyramidPositive(y, z);
                triangles[index].vertexs[1] = getPyramidPositive(y, z + dz);
                triangles[index].vertexs[2] = getPyramidPositive(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;


                triangles[index].vertexs[0] = getPyramidPositive(y, z);
                triangles[index].vertexs[1] = getPyramidPositive(y + dy, z);
                triangles[index].vertexs[2] = getPyramidPositive(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;
            }
            z += dz;
        }
        y += dy;
    }

    y = yStart;
    for (int yCount = 0; yCount < (yEnd - yStart) / dy; yCount++)
    {
        z = zStart;
        for (int zCount = 0; zCount < (zEnd - zStart) / dz; zCount++)
        {
            if ((z + dz) * (y + dy) < 0)
            {
                triangles[index].vertexs[0] = getPyramidNegative(y, z);
                triangles[index].vertexs[1] = getPyramidNegative(y, z + dz);
                triangles[index].vertexs[2] = getPyramidNegative(y + dy, z);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;


                triangles[index].vertexs[0] = getPyramidNegative(y, z + dz);
                triangles[index].vertexs[1] = getPyramidNegative(y + dy, z + dz);
                triangles[index].vertexs[2] = getPyramidNegative(y + dy, z);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;
            }
            else
            {
                triangles[index].vertexs[0] = getPyramidNegative(y, z);
                triangles[index].vertexs[1] = getPyramidNegative(y, z + dz);
                triangles[index].vertexs[2] = getPyramidNegative(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;


                triangles[index].vertexs[0] = getPyramidNegative(y, z);
                triangles[index].vertexs[1] = getPyramidNegative(y + dy, z);
                triangles[index].vertexs[2] = getPyramidNegative(y + dy, z + dz);
                triangles[index].calcTriangleNorm();
                triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

                index++;
            }
            z += dz;
        }
        y += dy;
    }
}

Vector3D getSpherePositive(double y, double z)
{
    return Vector3D(sqrt(pow(R, 2) - pow(y, 2) - pow(z, 2)), y, z);
}

Vector3D getSphereNegative(double y, double z)
{
    return Vector3D(-sqrt(pow(R, 2) - pow(y, 2) - pow(z, 2)), y, z);
}

void paintSphere()
{
    if (triangles == NULL)
        triangles = new Triangle[2 * (2 * PI / dTheta + 2) * (PI / dPhi + 2)];

    index = 0;

    double theta = 0, phi;

    for (int thetaCounter = 0; thetaCounter <= PI / dTheta; thetaCounter++)
    {
        phi = 0;
        theta += dTheta;
        for (int phiCounter = 0; phiCounter <= 2 * PI / dPhi; phiCounter++)
        {
            phi += dPhi;
            triangles[index].vertexs[0] = getSphere(phi, theta);
            triangles[index].vertexs[1] = getSphere(phi, theta + dTheta);
            triangles[index].vertexs[2] = getSphere(phi + dPhi, theta);
            triangles[index].calcTriangleNorm();
            triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

            index++;


            triangles[index].vertexs[0] = getSphere(phi, theta + dTheta);
            triangles[index].vertexs[1] = getSphere(phi + dPhi, theta + dTheta);
            triangles[index].vertexs[2] = getSphere(phi + dPhi, theta);
            triangles[index].calcTriangleNorm();
            triangles[index].brush = CreateSolidBrush(RGB(pow(triangles[index].norm.dotMul(camera.dir), 2) * 255, 0, 0));

            index++;
        }
    }
}

void sortZ(Triangle* triangle, int size)
{
    Triangle temp;
    int item;
    for (int counter = 1; counter < size; counter++)
    {
        temp = triangle[counter];
        item = counter - 1;
        while (item >= 0 && triangle[item].distanceToCamera(camera) < temp.distanceToCamera(camera)) 
        {
            triangle[item + 1] = triangle[item];
            triangle[item] = temp;
            item--;
        }
    }
}

