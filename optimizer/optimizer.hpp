#ifndef QZWLECR_OPTIMIZER_H
#define QZWLECR_OPTIMIZER_H
#include"../stdafx.hpp"
#include<queue>
#include<iostream>
#include<map>

using namespace cimg_library;
typedef unsigned char uchar;

extern int eps;
extern float face_size;
extern float background_size;
const int dx[]={0,0,1,1,1,-1,-1,-1};
const int dy[]={1,-1,0,1,-1,0,1,-1};

class readyToChange
{
public:
    readyToChange(int x_,int y_,cimg_color &data_)
    {x=x_,y=y_,data=data_;}
    int x;
    int y;
    cimg_color data;
};

std::vector<std::vector<bool> > doColorOptimize(CImg<unsigned char> &originImage,CImg<unsigned char> &colorOptimizedImage);

#endif
