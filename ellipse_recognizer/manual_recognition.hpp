#ifndef Q_MANUAL_ELLIPSE_RECOG_H
#define Q_MANUAL_ELLIPSE_RECOG_H

#include "../stdafx.hpp"
#include <vector>
#include <list>

namespace qLibrary{
    namespace Graphics{
        const int MIN_ELLIPSE_LAXIS = 30;
        const int MIN_ELLIPSE_SAXIS = 20;
        const int MIN_VOTES = 20;
        class qPoint2{
            public:
                int x;
                int y;
                qPoint2();
                qPoint2(int x,int y);
                void tosp(qPoint2 screen);
                void tocp(qPoint2 screen);
                double operator-(const qPoint2 &a);
                bool operator==(const qPoint2 &a);
                qPoint2 operator%(const qPoint2 &a);
        };
        class qEllipse{
            public:
                qPoint2 la1,la2,center;
                // la1: one end of long axis
                // la2: another end of long axis
                // center: self-explained
                int lhaxis,shaxis;
                // lhaxis: half of the length of long axis
                // shaxis: half of the length of short axis
                double rotateAngle;
                // rotateAngle: self-explained
                qEllipse();
                qEllipse(qPoint2 la1,qPoint2 la2); // WARNING: this method will make the ellipse a unsatisfied one
                qEllipse(qPoint2 la1,qPoint2 la2,qPoint2 another);
                void append(qPoint2 another);
        };
        class qEllipseComparator{
            public:
                bool operator()(const qEllipse &a,const qEllipse &b);
        };
        class qEllipseStorager{
            public:
                qEllipse* ellipse;
                int vote;
                std::list<qPoint2> followed_points;
                qEllipseStorager();
                ~qEllipseStorager();
        };
        std::vector<qEllipse> recognize_ellipse(cimg_library::CImg<unsigned char> &coroutine);
    }
}

#endif

