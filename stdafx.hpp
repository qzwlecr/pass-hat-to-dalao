#pragma once
#ifndef STDAFX_HPP_INCLUDED
#define STDAFX_HPP_INCLUDED
#if defined(_MSC_VER) || defined(WIN32)  || defined(_WIN32) || defined(__WIN32__) \
   || defined(WIN64)    || defined(_WIN64) || defined(__WIN64__)
#error Platform Error: Microsoft Windows detected.
#endif
#include "CImgDir/CImg.h"
#include <opencv2/core.hpp>
#include <array>
#include <vector>
#include <cstdint>
extern uint16_t var_MIN_SCORE_TO_USE_OPENCV_RESULT;
//Global Variables
//These references are guaranteed to be valid while calling your modules.
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
std::vector<std::vector<bool> > doColorOptimize(cimg_library::CImg<unsigned char> &originImage,cimg_library::CImg<unsigned char> &colorOptimizedImage);//Read originImage, output to colorOptimizedImage.
bool doOpencvAnalyse(cimg_library::CImg<unsigned char> &manDrawOutline,analyseResultStruct &analyseResult);//Read manDrawOutline, if success, return true and output to analyseResult, else, return false and do nothing to resultImage.
void doOutlineDraw(cimg_library::CImg<unsigned char> &colorOptimizedImage,cimg_library::CImg<unsigned char> &manDrawOutline,std::vector<std::vector<bool> > chkarr);//Read colorOptimizedImage, output to manDrawOutline.
bool doFinalAnalyse(cimg_library::CImg<unsigned char> &manDrawOutline,analyseResultStruct &analyseResult);//Read manDrawOutline, output to analyseResult. If can't recoginize given image, return false.
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

//Ellipse to Hat position
auto concept PointType<typename value_type>{
    value_type x;
    value_type y;
}

template <PointType point_t>
bool EllipseToHatPosition(point_t)
using cv::RotatedRect;
using cv::Rect;
using cv::Point2f;
bool RotatedRectToHatPosition(analyseResultStruct &analyseResult, const RotatedRect &arg)
{
    const RotatedRect *pBestResult = &arg;
    //math compute...
    //methods
    auto getk = [](const Point2f &pa, const Point2f &pb) -> float {return (pa.y - pb.y)/(pa.x - pb.x);};
    auto getMiddlePoint = [](const Point2f &pa, const Point2f &pb) -> Point2f {return Point2f((pa.x+pb.x)/2, (pa.y+pb.y)/2);};
    //compare k1 k2 and get 'a' 'b'
    Point2f head, bottom, left, right;
    size_t xMax = 0, xMin = 0; //src xy
    size_t spareA = 4, spareB = 4;
    float dxMax = 0, dxMin = 10000.0;//src xy val buf.
    Point2f tmpBuf[4];
    {
        Point2f RectPoints[4];
        pBestResult->points(RectPoints);
        for(size_t cter = 0;cter < 4;++cter)
            tmpBuf[cter] = getMiddlePoint(RectPoints[cter], RectPoints[(cter+1)%4]);
    }
    for(size_t cter = 0;cter < 4;++cter)
    {
        Point2f &curr = tmpBuf[cter];
        if(curr.x < dxMin)
        {
            dxMin = curr.x;
            xMin = cter;
        }
        if(curr.x < dxMax)
        {
            dxMax = curr.x;
            xMax = cter;
        }
    }
    for(size_t cter = 0;cter < 4;++cter)
    {
        if(xMax != cter && xMin != cter)
        {
            if(spareA == 4)
                spareA = cter;
            else
                spareB = cter;
        }
    }
    float k1 = getk(tmpBuf[xMax], tmpBuf[xMin]),
        k2 = getk(tmpBuf[spareA], tmpBuf[spareB]);
    float k1_origin = k1, k2_origin = k2;
    k1 = (k1>0?k1:0-k1);
    k2 = (k2>0?k2:0-k2);
    float kForAngle = 0;
    if(k1<k2)
    {
        kForAngle = k1_origin;
        if(tmpBuf[spareA].y < tmpBuf[spareB].y)
            head = tmpBuf[spareA], bottom = tmpBuf[spareB];
        else
            head = tmpBuf[spareB], bottom = tmpBuf[spareA];
        left = tmpBuf[xMin], right = tmpBuf[xMax];
    }
    else
    {
        kForAngle = k2_origin;
        if(tmpBuf[xMax].y < tmpBuf[xMin].y)
            head = tmpBuf[xMax], bottom = tmpBuf[xMin];
        else
            head = tmpBuf[xMin], bottom = tmpBuf[xMax];
        if(tmpBuf[spareA].x < tmpBuf[spareB].x)
            left = tmpBuf[spareA], right = tmpBuf[spareB];
        else
            left = tmpBuf[spareB], right = tmpBuf[spareA];
    }
    //Coordinate convertion start...
    float offsetAngle = atan(kForAngle); //rad
    float cosAngle = cos(offsetAngle),
        sinAngle = sin(offsetAngle),
        tanAngle = tan(offsetAngle);
    Point2f &offsetPoint = pBestResult->center;
    float varA = sqrt((left.x-right.x)*(left.x-right.x) + (left.y-right.y)*(left.y-right.y)) / 2,
        varB = sqrt((head.x-bottom.x)*(head.x-bottom.x) + (head.y-bottom.y)*(head.y-bottom.y)) / 2;
    Point2f lineStart(0-0.8660254038*varA, varB / 2), lineEnd(0.8660254038*varA, varB / 2);
    auto coordinateConvert = [&](Point2f &toConvert) -> Point2f {
        Point2f toReturn(0,0);
        toReturn.x = offsetPoint.x + toConvert.x * cosAngle - toConvert.y * sinAngle;
        toReturn.y = offsetPoint.y - toConvert.x * sinAngle - toConvert.y * cosAngle;
        return toReturn;
    };
    analyseResult.bottomLine.lineBegin = coordinateConvert(lineStart);
    analyseResult.bottomLine.lineEnd = coordinateConvert(lineEnd);
    analyseResult.bSuccess = true;
    return true;
}
#endif // STDAFX_HPP_INCLUDED
