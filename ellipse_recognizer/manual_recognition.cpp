#include "manual_recognition.hpp"
#include "math.h"
#include <set>
#include <vector>
#include <array>

double sq(double x){
    return x*x;
}

namespace qLibrary{
    namespace Graphics{
        double qPoint2::operator-(const qPoint2 &a){
            return sqrt(sq(a.x-this->x)+sq(a.y-this->y));
        }
        bool qPoint2::operator==(const qPoint2 &a){
            return ((this->x==a.x) and (this->y==a.y));
        }
        qPoint2 qPoint2::operator%(const qPoint2 &a){
            qPoint2 tmp((this->x+a.x),(this->y+a.y));
            return tmp;
        }
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
        qEllipse::qEllipse() : la1(0,0),la2(0,0),center(0,0){
            lhaxis=0;shaxis=0;rotateAngle=0.0;
        }
        qEllipse::qEllipse(qPoint2 la1,qPoint2 la2){
            // in this constructor, the following parameter can get
            this->la1=la1;
            this->la2=la2;
            center.x=(la1.x+la2.x)/2;
            center.y=(la1.y+la2.y)/2;
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
            center.x=(la1.x+la2.x)/2;
            center.y=(la1.y+la2.y)/2;
            lhaxis=sqrt(sq(la1.x-la2.x)+sq(la1.y-la2.y))/2;
            rotateAngle=atan(((double)(la2.y-la1.y))/((double)(la2.x-la1.x)));
            this->append(another);
        }
        bool qEllipseComparator::operator()(const qEllipse &a,const qEllipse &b){
            return a.shaxis<=b.shaxis;
        }
        qEllipseStorager::qEllipseStorager(){
            ellipse=NULL;
            vote=0;
        }
        qEllipseStorager::~qEllipseStorager(){
            delete ellipse;
        }
        std::vector<qEllipse> recognize_ellipse(cimg_library::CImg<unsigned char> &coroutine){
            int cross=(int)sqrt(sq(coroutine.width())+sq(coroutine.height()))+1;
            std::array<qEllipseStorager,cross> ellipses;
            bool checkArr[coroutine.width()][coroutine.height()];
            std::vector<qEllipse> output;
            // clear array
            qPoint2 screen_embody(coroutine.width(),coroutine.height());
            for(int iterx=0;iterx<coroutine.width();iterx++){
                for(int itery=0;itery<coroutine.height();itery++){
                    checkArr[iterx][itery]=true;
                }
            }
            std::vector<qPoint2> coroutinePoints;
            for(int iterx=0;iterx<coroutine.width();iterx++){
                for(int itery=0;itery<coroutine.height();itery++){
                    if(coroutine(iterx,itery,0,0)){
                        qPoint2 tmp(iterx,itery);
                        tmp.tocp(screen_embody);
                        coroutinePoints.push_back(tmp);
                    }
                }
            }

            for(auto &la1 : coroutinePoints){
                if(!checkArr[la1.x][la1.y])
                    continue;
                for(auto &la2 : coroutinePoints){
                    if(la1.x==la2.x and la1.y==la2.y)
                        continue;
                    if(!checkArr[la2.x][la2.y])
                        continue;
                    if((la1-la2)<MIN_ELLIPSE_LAXIS)
                        continue;
                    // all param check completed,start ellipse generation
                    for(auto &another : coroutinePoints){
                        if((another==la1) or (another==la2))
                            continue;
                        if(!checkArr[another.x][another.y])
                            continue;
                        if((la1%la2)-another<MIN_ELLIPSE_SAXIS)
                            continue;
                        // 3 points get,start voting
                        qEllipse *ellptr=new qEllipse(la1,la2);
                        ellptr->append(another);
                        if(ellipses[ellptr->shaxis].ellipse!=NULL){
                            // already have
                            ellipses[ellptr->shaxis].votes+=1;
                            ellipses[ellptr->shaxis].followed_points.push_back(another);
                            delete ellptr;
                        }else{
                            ellipses[ellptr->shaxis].ellipse=ellptr;
                            ellipses[ellptr->shaxis].votes=1;
                            ellipses[ellptr->shaxis].followed_points.push_back(la1);
                            ellipses[ellptr->shaxis].followed_points.push_back(la2);
                            ellipses[ellptr->shaxis].followed_points.push_back(another);
                        }
                    }
                    // all another points checked.
                    // now start checking votes.
                    for(int iter=0;iter<ellipses.size();iter++){
                        if(ellipses[iter].ellipse==NULL)
                            continue;
                        if(ellipses[iter].votes>MIN_VOTES){
                            qEllipse tmpell=*(ellipse[iter].ellipse);
                            output.push_back(tmpell);
                            for( auto pts : ellipses[iter].followed_points ){
                                checkArr[pts.x][pts.y]=false;
                            }
                        }
                    }
                    // votes check complete
                    // now clear arrays
                    ellipses.clear();

                }
            }
            return output;
        }
    }
}
