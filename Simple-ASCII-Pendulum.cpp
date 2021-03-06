#include <iostream>
#include <cmath>
#include <Windows.h>
#include <vector>
using namespace std;
// These two variables control the size of the window.
const int xSize = 100;
const int ySize = 100;

// These two variables control the position of the pendulum within the window.
const int offsetx = 25;
const int offsety = -32;

// This is the gravity and PI constant.
const float g = 0.01;
const float pi = 3.1415926535;

// These two variables control the time-step and dampening, respectively.
const float dt = 0.3;
const float DAMPENING = 0.998;

//These variables handle writing characters directly to the console.
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwWritten;

struct Pendulum
{
    const float length = 25;
    float angle = -2 * pi / 3;
    float x;
    float y;
    float v = 0;
    float a = 0;
    const float m = 2;
};

void plotLineLow(vector<pair<int, int>> &v, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;
    int y = y0;

    for (int x = x0; x <= x1; x++)
    {
        v.push_back(make_pair(x, y));
        if (D > 0)
        {
            y += yi;
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

void plotLineHigh(vector<pair<int, int>> &v, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }
    int D = 2 * dx - dy;
    int x = x0;

    for (int y = y0; y <= y1; y++)
    {
        v.push_back(make_pair(x, y));

        if (D > 0)
        {
            x += xi;
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}
//Bresenham's algorithm for drawing line
vector<pair<int, int>> giveCoords(int x0, int y0, int x1, int y1)
{
    vector<pair<int, int>> v;

    if (abs(y1 - y0) < abs(x1 - x0))
    {
        if (x0 > x1)
        {
            plotLineLow(v, x1, y1, x0, y0);
        }
        else
        {
            plotLineLow(v, x0, y0, x1, y1);
        }
    }
    else
    {
        if (y0 > y1)
        {
            plotLineHigh(v, x1, y1, x0, y0);
        }
        else
        {
            plotLineHigh(v, x0, y0, x1, y1);
        }
    }
    return v;
}
void clrscr()
{
    Sleep(10);
    COORD topLeft = {0, 0};
    DWORD cCharsWritten, dwConSize;
    CONSOLE_SCREEN_BUFFER_INFO cInfo;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the number of character cells in the current buffer
    GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);
    dwConSize = cInfo.dwSize.X * cInfo.dwSize.Y;

    // Fill the whole screen with blank chars
    FillConsoleOutputCharacterW(hConsoleOutput, ' ', dwConSize, topLeft, &cCharsWritten);

    // Get the current text attribute
    GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);

    // Set the buffer's attributes accordingly
    FillConsoleOutputAttribute(hConsoleOutput, cInfo.wAttributes, dwConSize, topLeft, &cCharsWritten);

    // Put the cursor at its home coordinates
    SetConsoleCursorPosition(hConsoleOutput, topLeft);
}
void drawBOB(float xp, float yp, char a)
{
    const int x[7] = {-1, 1, -2, 0, 2, -1, 1};
    const int y[7] = {0, 0, 1, 1, 1, 2, 2};
    for (int i = 0; i < 7; i++)
    {
        FillConsoleOutputCharacterA(hOutput, a, 1, {(short)(offsetx + (xSize / 2) + x[i] + xp), (short)(offsety + (ySize / 2) + y[i] + yp)}, &dwWritten);
    }
}
void drawPendArm(vector<pair<int, int>> &v, char a)
{
    for (auto i : v)
    {
        FillConsoleOutputCharacterA(hOutput, a, 1, {(short)(offsetx + xSize / 2 + i.first), (short)(ySize / 2 + offsety + i.second)}, &dwWritten);
    }
}
void displayINFO(float a, float b, float c)
{
    printf("\n\n\n                            Acceleration = %f\n", a);
    printf("                            Angle = %f\n", (b * 180) / pi);
    printf("                            Velocity = %f\n", c);
}
int main()
{
    Pendulum p;
    while (true)
    {
        
        p.x = p.length * sin(p.angle);
        p.y = p.length * cos(p.angle);

        float force = g * sin(p.angle);
        p.a = -1 * force;
        p.v = (p.v + p.a) * DAMPENING;
        p.angle += p.v;

        vector<pair<int, int>> v;

        v = giveCoords(0, 0, p.x, p.y);
        FillConsoleOutputCharacterA(hOutput, char(219), 1, {offsetx + (xSize / 2), (ySize / 2) - 1 + offsety}, &dwWritten);
        displayINFO(p.a, p.angle, p.v);
        drawPendArm(v, '*');
        drawBOB(p.x, p.y, char(219));
        clrscr();
        
    }
    return 0;
}
