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
            cv::Mat copy_img;
            cv::Mat circle_img=origimg;
            //cv::Mat grayimg,bin_img;
            cv::cvtColor(origimg,copy_img,CV_BGR2GRAY);
            std::vector<std::vector<cv::Point> > ellipses;
            //cv::threshold(grayimg,bin_img,0,255,cv::THRESH_BINARY|cv::THRESH_OTSU);// can be replaced by self_made binary_made func(see outliner)
            cv::findContours(copy_img,ellipses,CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
            for (auto &each : ellipses){
                unsigned int counts= each.size();// how much points contains.
                if(counts<100||counts>1000)
                    continue;
                cv::Mat alike_ellipses;
                cv::Mat(each).convertTo(alike_ellipses,CV_32F);
                cv::RotatedRect recresult=cv::fitEllipse(alike_ellipses);
                if(!checkDouble(recresult.size.width,0.0)){
                    outvec.push_back(recresult);
            //        cv::ellipse(circle_img,recresult,cv::Scalar(0,0,255),2,CV_AA);
                }
            }
            //cimg_library::CImg<unsigned char> recolic233(circle_img);
            //recolic233.display();
            return outvec;
        }

    }
}
