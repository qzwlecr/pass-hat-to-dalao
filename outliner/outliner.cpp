#include "outliner.hpp"
#include <iostream>


#define COLOR_R(cddimg,x,y) (*(cddimg.data(x,y,0,0)))
#define COLOR_G(cddimg,x,y) (*(cddimg.data(x,y,0,1)))
#define COLOR_B(cddimg,x,y) (*(cddimg.data(x,y,0,2)))
// notice the return val of cimg.data() is pointer.
// so it need to be de-referenced

namespace qLibrary{
    namespace Graphics{
        cimg_library::cimg_color getColorAt(cimg_library::CImg<unsigned char> cimg,int x,int y){
			//std::cout << "attempt call getColorAt() " << std::endl;
            cimg_library::cimg_color tmpcc(COLOR_R(cimg,x,y),COLOR_G(cimg,x,y),COLOR_B(cimg,x,y));
			//std::cout << "attemption over" << std::endl;
            return tmpcc;
        }
        bool checkColorDivergence(cimg_library::cimg_color a,cimg_library::cimg_color b){
            return ((a-b)>=MIN_DIVERGENCE);
        }
        bool checkOutlineAt(cimg_library::CImg<unsigned char> &orig,cimg_library::CImg<unsigned char> &out,int x,int y){
            // check the upper one only.
            // that can made the decision.
            if(y==0 or x==0)
                return false;// attempt to check the upper bound is not allowed.
            cimg_library::cimg_color currc(getColorAt(orig,x,y)),upperc(getColorAt(orig,x,y-1)),lefterc(getColorAt(orig,x-1,y));
            if(checkColorDivergence(currc,upperc) or checkColorDivergence(currc,lefterc)){
                out(x,y,0,0)=255;
                out(x,y,0,1)=255;
                out(x,y,0,2)=255;
                return true;
            }
            return false;
        }
        void checkOutline(cimg_library::CImg<unsigned char> &cimg,cimg_library::CImg<unsigned char> &outimg){
            int origwidth=cimg.width(),origlength=cimg.height();
			std::cout << "width " << origwidth << " length " << origlength << std::endl;
            for(int iterx=0;iterx<origwidth;iterx++){
                for(int itery=0;itery<origlength;itery++){
                    checkOutlineAt(cimg,outimg,iterx,itery);
                }
            }
        }
    }
}

void doOutlineDraw(cimg_library::CImg<unsigned char> &colorOptimizedImage,cimg_library::CImg<unsigned char> &manDrawOutline)
{
    qLibrary::Graphics::checkOutline(colorOptimizedImage,manDrawOutline);
}

