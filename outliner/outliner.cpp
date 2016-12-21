#include "outliner.hpp"

#define COLOR_R(cimg,x,y) (*(cimg.data(x,y,0,0)))
#define COLOR_G(cimg,x,y) (*(cimg.data(x,y,0,1)))
#define COLOR_B(cimg,x,y) (*(cimg.data(x,y,0,2)))
// notice the return val of cimg.data() is pointer.
// so it need to be de-referenced

namespace qLibrary{
    namespace Graphics{
        cimg_library::cimg_color getColorAt(cimg_library::CImg<unsigned char> cimg,int x,int y){
            cimg_library::cimg_color tmpcc(COLOR_R(cimg,x,y),COLOR_G(cimg,x,y),COLOR_B(cimg,x,y));
            return tmpcc;
        }
        bool checkColorDivergence(cimg_library::cimg_color a,cimg_library::cimg_color b){
            const int min_divergence=64;
            return ((a-b)>=min_divergence);
        }
        bool checkOutlineAt(cimg_library::CImg<unsigned char> &orig,cimg_library::CImg<unsigned char> &out,int x,int y){
            // check the upper one only.
            // that can made the decision.
            if(x==0)
                return false;// attempt to check the upper bound is not allowed.
            cimg_library::cimg_color currc(getColorAt(orig,x,y)),upperc(getColorAt(orig,x-1,y));
            if(checkColorDivergence(currc,upperc)){
                COLOR_R(out,x,y)=255;
                COLOR_G(out,x,y)=255;
                COLOR_B(out,x,y)=255;
                return true;
            }
            return false;
        }
    }
}
