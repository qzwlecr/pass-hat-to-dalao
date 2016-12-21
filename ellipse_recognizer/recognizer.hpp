#ifndef Q_ELLIPSE_RECOGNIZER_H
#define Q_ELLIPSE_RECOGNIZER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

namespace qLibrary{
    namespace Graphics{
        std::vector<cv::RotatedRect> doOpencvRecognizer(cv::Mat &colorized_img);
    }
    
}



#endif
