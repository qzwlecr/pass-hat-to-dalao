#pragma once
#ifndef STDAFX_HPP_INCLUDED
#define STDAFX_HPP_INCLUDED
#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
   || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
#error Platform Error: Microsoft Windows detected.
#endif
#include "CImgDir/CImg.h"
#include <array>
//Global Variables
//These references are guaranteed to be valid while calling your modules.

//Pass result
struct analyseResultStruct
	{
		bool bSuccess;
		struct {
			struct point {
				uint16_t x;
				uint16_t y;
			};
			point lineBegin;
			point lineEnd;
		} bottomLine;
};
//Prototypes of function (to complete)
//Assume thar 'originImage' is initialized properly.
void doColorOptimize(cimg_library::CImg<unsigned char> &originImage,cimg_library::CImg<unsigned char> &colorOptimizedImage);//Read originImage, output to colorOptimizedImage.
bool doOpencvAnalyse(cimg_library::CImg<unsigned char> &colorOptimizedImage,struct analyseResultStruct &analyseResult);//Read colorOptimizedImage, if success, return true and output to analyseResult, else, return false and do nothing to resultImage.
void doOutlineDraw(cimg_library::CImg<unsigned char> &colorOptimizedImage,cimg_library::CImg<unsigned char> &manDrawOutline);//Read colorOptimizedImage, output to manDrawOutline.
bool doFinalAnalyse(cimg_library::CImg<unsigned char> &manDrawOutline,struct analyseResultStruct &analyseResult);//Read manDrawOutline, output to analyseResult. If can't recoginize given image, return false.
void putHatOn(cimg_library::CImg<unsigned char> &originImage);//Select an hat and put it to originImage(according to analyseResult).
namespace cimg_library{
class cimg_color;
namespace recolic_private_namespace{
inline uint16_t colorCompare(const cimg_color &colorA, const cimg_color &colorB);//The greater it returns, the less similar input colors are.
}
//Public function
class cimg_color
{
public:
    cimg_color(unsigned char r=0, unsigned char g=0, unsigned char b=0) {color_dat={r,g,b};}
    cimg_color(const unsigned char *tcp) {for(size_t c=0;c<3;++c) color_dat[c]=tcp[c];}
    operator unsigned char *()
    {
        return color_dat.data();
    }
    uint16_t operator-(const cimg_color &another) {return recolic_private_namespace::colorCompare(*this, another);}
    std::array<unsigned char, 3> color_dat;
};
namespace recolic_private_namespace{
    inline uint16_t colorCompare(const cimg_color &colorA, const cimg_color &colorB)
    {
        auto getMinusAbs = [](unsigned char a, unsigned char b) -> uint16_t {return a>b?a-b:b-a;};
        return getMinusAbs(colorA.color_dat[0], colorB.color_dat[0])+getMinusAbs(colorA.color_dat[1], colorB.color_dat[1])+getMinusAbs(colorA.color_dat[2], colorB.color_dat[2]);
    }
}
}
#endif // STDAFX_HPP_INCLUDED
