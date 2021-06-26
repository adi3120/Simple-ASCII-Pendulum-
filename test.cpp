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
const double pi = 3.14;
// These two variables control the time-step and dampening, respectively.
const float dt = 0.3;
const float DAMPENING = 0.997;

//These variables handle writing characters directly to the console.
HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
DWORD dwWritten;
HWND myconsole = GetConsoleWindow();
HDC dc = GetDC(myconsole);


struct Pendulum
{
    float length = 25;
    float angle = pi / 3;
    float x = length * sin(angle);
    float y = length * cos(angle);
    float v = 0;
    float a = 0;
    float m = 2;
};

void plotLineLow(vector<pair<int, int>> &v, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0, dy = y1 - y0, yi = 1;
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
            D = D + (2 * (dy - dx));
        }
        else
            D += 2 * dy;
    }
}

void plotLineHigh(vector<pair<int, int>> &v, int x0, int y0, int x1, int y1)
{
    int dx = x1 - x0, dy = y1 - y0, xi = 1;
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
            D += 2 * (dx - dy);
        }
        D += 2 * dx;
    }
}
//Bresenham's algorithm for drawing line
vector<pair<int, int>> giveCoords(int x0, int y0, int x1, int y1)
{
    vector<pair<int, int>> v;
    if (x0 > x1)
    {
        int t;
        t = x0;
        x0 = x1;
        x1 = t;
        t = y0;
        y0 = y1;
        y1 = t;
    }
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
    Sleep(20);
    COORD topLeft = {0, 0};
    DWORD cCharsWritten, dwConSize;
    CONSOLE_SCREEN_BUFFER_INFO cInfo;
    HANDLE hConsoleOutput = GetStdHandle((DWORD)-11);

    // Get the number of character cells in the current buffer
    GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);
    dwConSize = cInfo.dwSize.X * cInfo.dwSize.Y;

    // Fill the whole screen with blank chars
    FillConsoleOutputCharacter(hConsoleOutput, (TCHAR)' ', dwConSize, topLeft, &cCharsWritten);

    // Get the current text attribute
    GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);

    // Set the buffer's attributes accordingly
    FillConsoleOutputAttribute(hConsoleOutput, cInfo.wAttributes, dwConSize, topLeft, &cCharsWritten);

    // Put the cursor at its home coordinates
    SetConsoleCursorPosition(hConsoleOutput, topLeft);
}
void drawBOB(vector<pair<int, int>> &v, char a)
{

    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 49 + v[v.size() - 1].first, offsety + 51 + v[v.size() - 1].second}, &dwWritten);
    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 51 + v[v.size() - 1].first, offsety + 51 + v[v.size() - 1].second}, &dwWritten);
    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 48 + v[v.size() - 1].first, offsety + 52 + v[v.size() - 1].second}, &dwWritten);
    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 50 + v[v.size() - 1].first, offsety + 52 + v[v.size() - 1].second}, &dwWritten);
    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 52 + v[v.size() - 1].first, offsety + 52 + v[v.size() - 1].second}, &dwWritten);
    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 49 + v[v.size() - 1].first, offsety + 53 + v[v.size() - 1].second}, &dwWritten);
    FillConsoleOutputCharacterW(hOutput, a, 1, {offsetx + 51 + v[v.size() - 1].first, offsety + 53 + v[v.size() - 1].second}, &dwWritten);
}
void drawPendArm(vector<pair<int, int>> &v, char a)
{

    for (int i = 0; i < v.size(); i++)
    {

        FillConsoleOutputCharacter(hOutput, a, 1, {offsetx + xSize / 2 + v[i].first, ySize / 2 + offsety + v[i].second}, &dwWritten);
    }
}
int main()
{
    Pendulum p;

    while (true)
    {
        p.x = p.length * sin(p.angle);
        p.y = p.length * cos(p.angle);
        if (p.angle == pi / 2)
        {
            p.y = 0;
        }
        if (p.angle == 0)
        {
            p.x = 0;
        }
        float force = (g * sin(p.angle));
        p.a = (-1 * force);
        p.v = (p.v + p.a) * DAMPENING;
        p.angle += p.v;
        vector<pair<int, int>> v;

        v = giveCoords(0, 0, p.x, p.y);
        FillConsoleOutputCharacter(hOutput, char(219), 1, {offsetx + 50, 49 + offsety}, &dwWritten);
        cout << "\n\n\n\n\n\n                                         Acceleration = " << p.a << endl;
        cout << "                                         Angle = " << (p.angle * 180) / pi << endl;

        drawPendArm(v, 'o');
        drawBOB(v, 'x');
        clrscr();
    }
}
