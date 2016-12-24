
#ifndef STDAFX_HPP_INCLUDED
#define STDAFX_HPP_INCLUDED
#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
   || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
#error Platform Error: Microsoft Windows detected.
#endif
#include "CImgDir/CImg.h"
#define cimg_use_jpeg
#include <opencv2/core.hpp>
#include <array>
using std::array;
#include <vector>
#include <cstdint>
extern uint16_t var_MIN_SCORE_TO_USE_OPENCV_RESULT;
extern bool var_ENABLE_GREEN;
extern float var_GREEN_RANDOM_RATE;
//Global Variables
//These references are guaranteed to be valid while calling your modules.
struct analyseResultStruct;
extern cimg_library::CImg<unsigned char> originImage;
extern analyseResultStruct analyseResult;
extern cimg_library::CImg<unsigned char> colorOptimizedImage,
    manDrawOutline,
    resultImage;
//Pass result
struct analyseResultStruct
	{
		bool bSuccess;
		struct {
			cv::Point2f lineBegin;
			cv::Point2f lineEnd;
		} bottomLine;
};
//Prototypes of function (to complete)
//Assume thar 'originImage' is initialized properly.
std::vector<std::vector<bool> > doColorOptimize();//Read originImage, output to colorOptimizedImage.
bool doOpencvAnalyse();//Read manDrawOutline, if success, return true and output to analyseResult, else, return false and do nothing to resultImage.
void doOutlineDraw(std::vector<std::vector<bool> > chkarr);//Read colorOptimizedImage, output to manDrawOutline.
bool doFinalAnalyse();//Read manDrawOutline, output to analyseResult. If can't recoginize given image, return false.
void putHatOn();//Select an hat and put it to originImage(according to analyseResult).
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

//Ellipse to Hat position
//auto concept PointType<typename value_type>{
//    value_type x;
//    value_type y;
//}

template <class point_t>
bool EllipseToHatPosition(array<point_t, 4> points);

#endif // STDAFX_HPP_INCLUDED
