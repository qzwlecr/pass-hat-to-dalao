#include "stdafx.hpp"
#include <iostream>
#include <string>
#include <vector>

cimg_library::CImg<unsigned char> originImage,
    colorOptimizedImage,
    manDrawOutline,
    resultImage;

using namespace std;
using namespace cimg_library;
void displayHelpInfo(){cout << "help"<<endl;}
int main(int argv_size, const char **args)
{
	cimg_library::CImg<unsigned char> originImage,
    colorOptimizedImage;
	bool** chkarr;
	analyseResultStruct analyseResult;
    if(argv_size < 2)
    {
        displayHelpInfo();
        return 0;
    }
    try{
        originImage = CImg<unsigned char>(args[1]);
    }
    catch(CImgIOException &e)
    {
        cout << "On opening the image:" << e.what() << endl;
        return 0;
    }
    //Additional arguments
    string outputFileName;
    enum argumentMeaning{ARG_NULL = 0, ARG_OUTPUT};
    size_t bNextArgumentMeaning = 0;
    for(size_t cter = 2;cter < argv_size; ++cter)
    {
        string currentArg(args[cter]);
        //do something.
        switch(bNextArgumentMeaning)
        {
        case ARG_NULL:
            if(currentArg == "-o")
                bNextArgumentMeaning = ARG_OUTPUT;
            else
            {
                cout << "Invalid argument " << currentArg << ". Check and try again." << endl;
                return 0;
            }
            break;
        case ARG_OUTPUT:
            outputFileName = currentArg;
            break;
        default:
            throw runtime_error("Argument analyzement error: switch case overflow.");
        }
    }

    //Do initialization.
    CImg<unsigned char> black_init(originImage);
    black_init.fill(255);
    colorOptimizedImage = manDrawOutline = resultImage = black_init;
//    try
//    {
		//originImage.display();
        chkarr=doColorOptimize(originImage,colorOptimizedImage);
        cout << "DEBUG-TERMINAGE:"<< endl;
        colorOptimizedImage.display();
        cin.get();
		cimg_library::CImg<unsigned char> manDrawOutline(originImage.width(),originImage.height(),1,3,0);
		doOutlineDraw(colorOptimizedImage,manDrawOutline,chkarr);
		manDrawOutline.display();
        cin.get();


        if(!doOpencvAnalyse(manDrawOutline, analyseResult))
        {
            //if(!doFinalAnalyse())
            {

                cout << "Image is refused to be recoginized, try another picture." << endl;
                return 0;
            }
        }
        cout << "Result:beginx=" << analyseResult.bottomLine.lineBegin.x << "beginy=" << analyseResult.bottomLine.lineBegin.y << endl;
        cout << "Result:endx=" << analyseResult.bottomLine.lineEnd.x << "endy=" << analyseResult.bottomLine.lineEnd.y << endl;
        /*putHatOn();
//    }
    if(outputFileName.empty())
        outputFileName = "output.png";
    resultImage.save_png(outputFileName.c_str());
    cout << "Done. Result saved to " << outputFileName << " successfully." << endl;



    return 0;*/
}
