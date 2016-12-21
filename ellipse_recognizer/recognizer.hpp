#ifndef Q_ELLIPSE_RECOGNIZER_H
#define Q_ELLIPSE_RECOGNIZER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include <opencv2/core.hpp>
#define cimg_plugin1 "cvMat.h"

#define MIN_SCORE_TO_USE_OPENCV_RESULT 100

namespace qLibrary{
    namespace Graphics{
        std::vector<cv::RotatedRect> doOpencvRecognizer(cv::Mat &colorized_img);
    }
}
using cv::RotatedRect;
using cv::Rect;
bool doOpencvAnalyse()
{
    auto recognizedBuf = qLibrary::Graphics::doOpencvRecognizer(colorOptimizedImage.get_MAT());
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
        if(currentScore && currentScore > maxScore)
        {
            maxScore = currentScore;
            pBestResult = &currentCheckRect;
        }
    }
    if(maxScore < MIN_SCORE_TO_USE_OPENCV_RESULT)
        return false;
    analyseResult.bSuccess = true;
    analyseResult.bottomLine.lineBegin =
}

#endif
