#include "../stdafx.hpp"
#include "recognizer.hpp"

namespace qLibrary{
    namespace Graphics{
        bool checkDouble(double a,double b){
            double divergence=a-b;
            return ((divergence<0.000001) and (divergence> -0.000001));
        }
        const unsigned int MAX_PCTG=50;
        const unsigned int MIN_PCTG=1000;
        std::vector<cv::RotatedRect> doOpencvRecognizer(const cv::Mat &origimg){
            std::vector<cv::RotatedRect> outvec;
            if(origimg.empty())
                return outvec;
            cv::Mat grayimg,bin_img;
            cv::cvtColor(origimg,grayimg,CV_BGR2GRAY);
            std::vector<std::vector<cv::Point> > ellipses;
            cv::threshold(grayimg,bin_img,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);// can be replaced by self_made binary_made func(see outliner)
            cv::findContours(bin_img,ellipses,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
            for (auto &each : ellipses){
                unsigned int counts= each.size();// how much points contains.
                cv::Mat alike_ellipses;
                cv::Mat(each).convertTo(alike_ellipses,CV_32F);
                cv::RotatedRect recresult=cv::fitEllipse(alike_ellipses);
                if(!checkDouble(recresult.size.width,0.0)){
                    outvec.push_back(recresult);
                }
            }
            return outvec;
        }

    }
}
