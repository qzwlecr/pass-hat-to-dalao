#include "stdafx.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace cimg_library;
void displayHelpInfo(){cout << "help"<<endl;}
int main(int argv_size, const char **args)
{
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
//    try
    {
        doColorOptimize();
        if(!doOpencvAnalyse())
        {
            doOutlineDraw();
            if(!doFinalAnalyse())
            {
                cout << "Image can't be recoginized, try another picture." << endl;
                return 0;
            }
        }
        putHatOn();
    }
    if(outputFileName.empty())
        outputFileName = "output.png";
    resultImage.save_png(outputFileName.c_str());
    cout << "Done. Result saved to " << outputFileName << " successfully." << endl;



    return 0;
}
