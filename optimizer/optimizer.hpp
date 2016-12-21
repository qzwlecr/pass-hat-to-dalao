#include"../stdafx.hpp"
#include<queue>
#include<map>

using namespace cimg_library;
typedef unsigned char uchar;
const int eps=10;
const int dx[]={0,0,1,1,1,-1,-1,-1};
const int dy[]={1,-1,0,1,-1,0,1,-1};

class readyToChange
{
public:
    readyToChange(int x_,int y_,cimg_color &data_):data(data_)
    {x=x_,y=y_;}
    int x;
    int y;
    cimg_color data;
};

void doColorOptimize();

