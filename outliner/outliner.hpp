#ifndef Q_OUTLINER_H
#define Q_OUTLINER_H

#include "../stdafx.hpp"

namespace qLibrary{
    namespace Graphics{
        const int MIN_DIVERGENCE = 64;
        cimg_library::cimg_color getColorAt(cimg_library::CImg<unsigned char> cimg,int xpos,int ypos);
        bool checkColorDivergence(cimg_library::cimg_color a,cimg_library::cimg_color b); // return true when the two color have distinct divergence
        bool checkOutlineAt(cimg_library::CImg<unsigned char> &origimg,cimg_library::CImg<unsigned char> &outimg,int xpos,int ypos); // if succeded, make that point BLACK and return TRUE
        void checkOutline(cimg_library::CImg<unsigned char> &cimg,cimg_library::CImg<unsigned char> &out);
    }
}

void doOutlineDraw(cimg_library::CImg<unsigned char> &colorOptimizedImage,cimg_library::CImg<unsigned char> &manDrawOutline,bool* chkArray);

#endif
