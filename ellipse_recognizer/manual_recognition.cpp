#include "manual_recognition.hpp"
#include "math.h"

double sq(double x){
    return x*x;
}

namespace qLibrary{
    namespace Graphics{
        qPoint2::qPoint2(){
            x=0;y=0;
        }
        qPoint2::qPoint2(int x,int y){
            this->x=x;
            this->y=y;
        }
        void qPoint2::tosp(qPoint2 screen) {
            y = screen.y - y;
        }
        void qPoint2::tocp(qPoint2 screen) {
            y = screen.y - y;
        }
        qRect2::qRect2() : xy(0,0),wh(0,0){}
        qRect2::qRect2(int x,int y,int w,int h) : xy(x,y),wh(w,h){}
        qRect2::qRect2(qPoint2 xy,qPoint2 wh){
            this->xy=xy;
            this->wh=wh;
        }
        qEllipse::qEllipse() : la1(0,0),la2(0,0),center(0,0){
            lhaxis=0;shaxis=0;rotateAngle=0.0;
        }
        qEllipse::qEllipse(qPoint2 la1,qPoint2 la2){
            // in this constructor, the following parameter can get
            this->la1=la1;
            this->la2=la2;
            center.x=(la1.x-la2.x)/2;
            center.y=(la1.y-la2.y)/2;
            lhaxis=sqrt(sq(la1.x-la2.x)+sq(la1.y-la2.y))/2;
            rotateAngle=atan(((double)(la2.y-la1.y))/((double)(la2.x-la1.x)));
        }
        void qEllipse::append(qPoint2 another){
            // WARNING: assume this ELLIPSE is constructed by (qp2,qp2)
            // or may get some fucking ERRORs.
            double d2=sq(another.x-center.x)+sq(another.y-center.y);
            double f2=sq(another.x-la2.x)+sq(another.y-la2.y);
            double cos2d=sq(((double)(sq(lhaxis)+d2-f2))/(2*(double)lhaxis*sqrt(d2)));
            double sin2d=1-cos2d;
            shaxis=sqrt((sq(lhaxis)*d2*sin2d)/(sq(lhaxis)-d2*cos2d));
        }
        qEllipse::qEllipse(qPoint2 la1,qPoint2 la2,qPoint2 another){
            this->la1=la1;
            this->la2=la2;
            center.x=(la1.x-la2.x)/2;
            center.y=(la1.y-la2.y)/2;
            lhaxis=sqrt(sq(la1.x-la2.x)+sq(la1.y-la2.y))/2;
            rotateAngle=atan(((double)(la2.y-la1.y))/((double)(la2.x-la1.x)));
            this->append(another);
        }
        qRect2 qEllipse::toRect(){
            
        }
    }
}
