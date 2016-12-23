#include "stdafx.hpp"
#include <iostream>
#include <string>
#include <vector>

//global variable
cimg_library::CImg<unsigned char> originImage;
//config list(alias defined at stdafx.hpp)
uint16_t var_MIN_SCORE_TO_USE_OPENCV_RESULT = 100; //default
int eps=20;
float face_size=0;
float background_size=0;

using namespace std;
using namespace cimg_library;
void displayHelpInfo(){cout << "help"<<endl;}
void readConfig();

int main(int argv_size, const char **args)
{
    std::vector<std::vector<bool> > chkarr;
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
    readConfig();
    //Do initialization.
    CImg<unsigned char> colorOptimizedImage(originImage.width(),originImage.height(),1,3,0),
    manDrawOutline(originImage.width(),originImage.height(),1,3,0),
    resultImage(originImage.width(),originImage.height(),1,3,0);
//    CImg<unsigned char> black_init(originImage);
//    black_init.fill(255);
//    colorOptimizedImage = manDrawOutline = resultImage = black_init;
//    try
//    {
		originImage.display();
		cout << "Press enter to continue:";
		cin.get();
        chkarr=doColorOptimize(originImage,colorOptimizedImage);
        cout << "DEBUG-TERMINAGE:"<< endl;
		CImg<unsigned char> boolArrayDisp(originImage.width(),originImage.height(),1,3,0);
		for(int iterx=0;iterx<originImage.width();iterx++){
			for(int itery=0;itery<originImage.height();itery++){
				if(chkarr[iterx][itery]){
					boolArrayDisp(iterx,itery,0,0)=255;
					boolArrayDisp(iterx,itery,0,1)=255;
					boolArrayDisp(iterx,itery,0,2)=255;
				}
			}
		}
        colorOptimizedImage.display();
		boolArrayDisp.display();
		doOutlineDraw(colorOptimizedImage,manDrawOutline,chkarr);
		manDrawOutline.display();


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

#include <fstream>
void readConfig()
{
    //Remain default if config file doesn't exist.
    ifstream is("config.cfg");
    if(!is)
        return;
    string cfgValBuf;
    char transBuffer[256];
    //Define methods.
    auto readCfgValueU16 = [](const string &buf, const string &recordName) -> pair<bool, uint16_t> {
        if(buf.size() < recordName.size() + 2)
            return make_pair(false, 0);
        if(buf.substr(0, recordName.size()) == recordName)
        {
            if(buf[recordName.size()] == '=')
                return make_pair(true, static_cast<uint16_t>(stoul(buf.substr(recordName.size() + 1))));
        }
        return make_pair(false, 0);
    };
    auto readCfgValueI = [](const string &buf, const string &recordName) -> pair<bool, int> {
        if(buf.size() < recordName.size() + 2)
            return make_pair(false, 0);
        if(buf.substr(0, recordName.size()) == recordName)
        {
            if(buf[recordName.size()] == '=')
                return make_pair(true, stoi(buf.substr(recordName.size() + 1)));
        }
        return make_pair(false, 0);
    };
    auto readCfgValueF = [](const string &buf, const string &recordName) -> pair<bool, float> {
        if(buf.size() < recordName.size() + 2)
            return make_pair(false, 0);
        if(buf.substr(0, recordName.size()) == recordName)
        {
            if(buf[recordName.size()] == '=')
                return make_pair(true, stof(buf.substr(recordName.size() + 1)));
        }
        return make_pair(false, 0);
    };

    while(!is.eof())
    {
        is.getline(transBuffer, 256);
        cfgValBuf = transBuffer;
        //Now, read and initialize config val.
        if(cfgValBuf.empty() || cfgValBuf[0] == '#')
            continue;
        auto checkedResult = readCfgValueU16(cfgValBuf, "MIN_SCORE_TO_USE_OPENCV_RESULT");
        if(checkedResult.first)
        {
            var_MIN_SCORE_TO_USE_OPENCV_RESULT = checkedResult.second;
            continue;
        }
        checkedResult = readCfgValueI(cfgValBuf, "eps");
        if(checkedResult.first)
        {
            eps = checkedResult.second;
            continue;
        }
        checkedResult = readCfgValueF(cfgValBuf, "face_size");
        if(checkedResult.first)
        {
            face_size = checkedResult.second;
            continue;
        }
        checkedResult = readCfgValueF(cfgValBuf, "background_size");
        if(checkedResult.first)
        {
            background_size = checkedResult.second;
            continue;
        }
    }
    return;
}
