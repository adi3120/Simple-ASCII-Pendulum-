#include <iostream>
#include <utility>
#include <vector>
using namespace std;
vector<pair<int, int>> drawline(int x0, int y0, int x1, int y1)
{
    vector<pair<int, int>> v;
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
    return v;
}
int main()
{
    vector<pair<int, int>> v;
    v = drawline(0, 1, 6, 4);
    for(int i=0;i<v.size();i++){
        cout<<v[i].first<<" "<<v[i].second<<" "<<endl;
    }
}