#include <iostream>
#include <cmath>
#include <Windows.h>
#include <vector>
using namespace std;

// These two variables control the size of the window.
int xSize = 100;
int ySize = 100;

// These two variables control the position of the pendulum within the window.
int offsetx = 25;
int offsety = -32;

// This is the gravity constant.
float g = 1;

// These two variables control the time-step and dampening, respectively.
float dt = 0.3;
float DAMPENING = 0.997;

// These variables handle writing characters directly to the console.
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwWritten;
HWND myconsole = GetConsoleWindow();
HDC dc = GetDC(myconsole);

struct Pendulum
{
    float length = 10;
    float angle = M_PI / 2;
    float x = length * sin(angle);
    float y = length * cos(angle);
    float v = 0;
    float a = 0;
    float m = 2;
};
vector<pair<int, int>> drawline(int x0, int y0, int x1, int y1)
{
    vector<pair<int, int>> v;
    int dx = x1 - x0, dy = y1 - y0;

    int D = 2 * dy - dx;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        v.push_back(make_pair(x, y));
        if (D > 0)
        {
            y += 1;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
    return v;
}
int main()
{

    Pendulum p;
    while (true)
    {
        system("CLS");
        p.x = p.length * sin(p.angle);
        p.y = p.length * cos(p.angle);
        vector<pair<int, int>> v;

        v = drawline(0, 0, p.x, p.y);
        FillConsoleOutputCharacter(hOutput, char(219), 1, {offsetx + xSize / 2, (ySize / 2) + offsety}, &dwWritten);

        //p.angle += 0.01;
        for (int i = 0; i < v.size(); i++)
        {
            FillConsoleOutputCharacter(hOutput, '=', 1, {(xSize / 2) + v[i].first, (ySize / 2) + v[i].second}, &dwWritten);
        }
    }
}
