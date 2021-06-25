#include <iostream>
#include <cmath>
#include <Windows.h>
#include <vector>
using namespace std;
#define pi 3.1415
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
    float length = 20;
    float angle = pi / 2;
    float x = length * sin(angle);
    float y = length * cos(angle);
    float v = 0;
    float a = 0;
    float m = 2;
};

void plotLineLow(vector<pair<int,int>> &v ,int x0, int y0, int x1, int y1)
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
            D -= 2 * dx;
        }
        D += 2 * dy;
    }
}

void plotLineHigh(vector<pair<int,int>> &v ,int x0, int y0, int x1, int y1)
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
            D -= 2 * dy;
        }
        D += 2 * dx;
    }
}

vector<pair<int,int>> drawLine(int x0, int y0, int x1, int y1)
{
    vector<pair<int, int>> v;
    //sorting
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
    // if(x0==x1){
    //     int min=y0,max=y1;
    //     if(y1<y0)
	// 	{
	// 		min=y1;
	// 		max=y0;
	// 	}
	// 	for(int i=min;i<=max;i++)
	// 		v.push_back(make_pair(x0,i));//platno[i][A]=c;
    // }
    // if(y0==y1)
	// {
	// 	for(int i=x0;i<=x1;i++)
	// 		v.push_back(make_pair(i,y0));//platno[B][i]=c;
	// }

    if (abs(y1 - y0) < abs(x1 - x0))
        plotLineLow(v,x0, x1, y0, y1);
    else
    {
        if (y0 > y1)
            plotLineHigh(v,x1, y1, x0, y0);
        else
            plotLineHigh(v,x0, y0, x1, y1);
    }
    return v;
}
void clrscr()
{
    COORD topLeft = {0, 0};
    DWORD cCharsWritten, dwConSize;
    CONSOLE_SCREEN_BUFFER_INFO cInfo;
    HANDLE hConsoleOutput = GetStdHandle((DWORD)-11);

    // Get the number of character cells in the current buffer
    GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);
    dwConSize = cInfo.dwSize.X * cInfo.dwSize.Y;

    // Fill the whole screen with blank chars
    // FillConsoleOutputCharacter(hConsoleOutput, (TCHAR)' ', dwConSize, topLeft, &cCharsWritten);

    // Get the current text attribute
    GetConsoleScreenBufferInfo(hConsoleOutput, &cInfo);

    // Set the buffer's attributes accordingly
    FillConsoleOutputAttribute(hConsoleOutput, cInfo.wAttributes, dwConSize, topLeft, &cCharsWritten);

    // Put the cursor at its home coordinates
    SetConsoleCursorPosition(hConsoleOutput, topLeft);
}
int main()
{

    Pendulum p;
    int a = 10;

    while (true)
    {   system("CLS");
        clrscr();
        p.x = p.length * sin(p.angle);
        p.y = p.length * cos(p.angle);
        vector<pair<int, int>> v;
        
        v = drawLine(0, 0, p.x, p.y);
        FillConsoleOutputCharacter(hOutput, char(219), 1, {offsetx + 50, 50 + offsety}, &dwWritten);
        Sleep(10);
        p.angle += 0.09;
        for (int i = 0; i < v.size(); i++)
        {
            //cout << v[i].first << " " << v[i].second << endl;
            FillConsoleOutputCharacter(hOutput, '#', 1, {offsetx-1 + xSize / 2 + v[i].first, ySize / 2 + offsety + v[i].second}, &dwWritten);
        }
    }
}
