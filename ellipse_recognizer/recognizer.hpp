#ifndef Q_ELLIPSE_RECOGNIZER_H
#define Q_ELLIPSE_RECOGNIZER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <cmath>
#include <iostream>

#include <opencv2/core.hpp>
#include "../stdafx.hpp"

#define MIN_SCORE_TO_USE_OPENCV_RESULT var_MIN_SCORE_TO_USE_OPENCV_RESULT

namespace qLibrary{
    namespace Graphics{
        std::vector<cv::RotatedRect> doOpencvRecognizer(const cv::Mat &colorized_img);
    }
}
using cv::RotatedRect;
using cv::Rect;
using cv::Point2f;
using std::endl;
using std::cout;

bool doOpencvAnalyse()//Read colorOptimizedImage, if success, return true and output to analyseResult, else, return false and do nothing to resultImage.
{
    cout << "***********************RECOLIC_DEBUG**************************" << endl;
    auto recognizedBuf = qLibrary::Graphics::doOpencvRecognizer(manDrawOutline.get_MAT());
    //DEBUG INFO
    cout << "BUFFER SIZE:" << recognizedBuf.size() << endl;

    auto printPoint = [](const Point2f &toPrint) {cout << '(' << toPrint.x << ',' << toPrint.y << ')'; };
    for(const RotatedRect &cur : recognizedBuf)
    {
        Point2f buf[4];
        cur.points(buf);
        cout << "RECOLIC_DEBUG > Center=" << (printPoint(cur.center), " Height=") << cur.boundingRect().height << " Width=" << cur.boundingRect().width << "4Point:";
        for(size_t cter = 0; cter < 4; ++cter)
            printPoint(buf[cter]);
        cout << "|" << endl;
    }
    //DEBUG INFO
    auto scoreRotatedRect = [](const RotatedRect &toJudge, int originHeight) -> uint16_t {
        //remove some abviously invalid results.
        const Rect &limitedRect = toJudge.boundingRect();
        if(limitedRect.height < originHeight/4 || limitedRect.height > originHeight)
            return 0;
        if(toJudge.center.y * 2 - limitedRect.height > originHeight)
            return 0;
        float unbalanceRate = limitedRect.height / limitedRect.width;
        if(unbalanceRate > 3 || unbalanceRate < 0.33)
            return 0;
        //give score.
        int16_t toReturn = (limitedRect.height + limitedRect.width) - toJudge.center.y;
        return toReturn > 0 ? static_cast<uint16_t>(toReturn) : 0;
    };
    uint16_t maxScore = 0;
    RotatedRect *pBestResult = nullptr;
    int originHeight = colorOptimizedImage.height();
    for(RotatedRect &currentCheckRect : recognizedBuf)
    {
        uint16_t currentScore = scoreRotatedRect(currentCheckRect, originHeight);
        cout << "? >" << currentScore << " got, height=" << currentCheckRect.boundingRect().height << endl;
        if(currentScore && currentScore > maxScore)
        {
            maxScore = currentScore;
            pBestResult = &currentCheckRect;
        }
    }
    if(maxScore < MIN_SCORE_TO_USE_OPENCV_RESULT)
        return false;
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
        if(curr.x > dxMax)
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
    if(std::isnan(k1))
    {
        k1 = (k2>0?0-100:100);
    }
    else if(std::isnan(k2))
    {
        k2 = (k1>0?0-100:100);
    }
    float k1_origin = k1, k2_origin = k2;
    k1 = (k1>0?k1:0-k1);
    k2 = (k2>0?k2:0-k2);
    cout << "CALC > absk1=" << k1 << ", absk2=" << k2 << endl;
    float kForAngle = 0;
    cout << "CALC > spareA=" << spareA << "spareB=" << spareB << "xMin=" << xMin << "xMax=" << xMax << endl;
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
    cout << "CALC >head";
    printPoint(head);
    cout << "bottom";
    printPoint(bottom);
    cout << "left";
    printPoint(left);
    cout << "right";
    printPoint(right);
    cout << endl;
    //Coordinate convertion start...
    float offsetAngle = atan(kForAngle); //rad
    cout << "CALC > offsetAngle=" << offsetAngle << endl;
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

template <class point_t>
bool EllipseToHatPosition(array<point_t, 4> tmpBuf)
{
    auto getk = [](const point_t &pa, const point_t &pb) -> float {return (pa.y - pb.y)/(pa.x - pb.x);};
    auto getMiddlePoint = [](const point_t &pa, const point_t &pb) -> point_t {return point_t((pa.x+pb.x)/2, (pa.y+pb.y)/2);};
    //compare k1 k2 and get 'a' 'b'
    point_t head, bottom, left, right;
    size_t xMax = 0, xMin = 0; //src xy
    size_t spareA = 4, spareB = 4;
    float dxMax = 0, dxMin = 10000.0;//src xy val buf.
    for(size_t cter = 0;cter < 4;++cter)
    {
        point_t &curr = tmpBuf[cter];
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
    point_t offsetPoint = getMiddlePoint(head, bottom);
    float varA = sqrt((left.x-right.x)*(left.x-right.x) + (left.y-right.y)*(left.y-right.y)) / 2;
    float varB = sqrt((head.x-bottom.x)*(head.x-bottom.x) + (head.y-bottom.y)*(head.y-bottom.y)) / 2;
    point_t lineStart(0-0.8660254038*varA, varB / 2), lineEnd(0.8660254038*varA, varB / 2);
    auto coordinateConvert = [&](point_t &toConvert) -> point_t {
        point_t toReturn(0,0);
        toReturn.x = offsetPoint.x + toConvert.x * cosAngle - toConvert.y * sinAngle;
        toReturn.y = offsetPoint.y - toConvert.x * sinAngle - toConvert.y * cosAngle;
        return toReturn;
    };
    analyseResult.bottomLine.lineBegin = coordinateConvert(lineStart);
    analyseResult.bottomLine.lineEnd = coordinateConvert(lineEnd);
    analyseResult.bSuccess = true;
    return true;
}/*
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
    const Point2f &offsetPoint = pBestResult->center;
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
*/
#endif
