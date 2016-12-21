#ifndef Q_ELLIPSE_RECOGNIZER_H
#define Q_ELLIPSE_RECOGNIZER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

#include <opencv2/core.hpp>
#define cimg_plugin1 "cvMat.h"

namespace qLibrary{
    namespace Graphics{
        std::vector<cv::RotatedRect> doOpencvRecognizer(cv::Mat &colorized_img);
    }
}

bool doOpencvAnalyse()
{
    auto recognizedBuf = qLibrary::Graphics::doOpencvRecognizer(colorOptimizedImage);

}

#endif
