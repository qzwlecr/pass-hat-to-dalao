#include "stdafx.hpp"
#include "put_hat_on.hpp"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using namespace cimg_library;

//global variable
CImg<unsigned char> originImage;
analyseResultStruct analyseResult;
CImg<unsigned char> colorOptimizedImage,
    manDrawOutline,
    resultImage;
pair<vector<string>, vector<string>> hatImageInfos;
//config list(alias defined at stdafx.hpp)
uint16_t var_MIN_SCORE_TO_USE_OPENCV_RESULT = 100; //default
int eps=20;
float face_size=0;
float background_size=0;
float blur_number=2.5;

void displayHelpInfo(){cout << "help"<<endl;}
void readConfig();

int main(int argv_size, const char **args)
{
    std::vector<std::vector<bool> > chkarr;

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
    CImg<unsigned char> black_init(originImage.width(),originImage.height(),1,3,0);
//    black_init.fill(255);
    colorOptimizedImage = manDrawOutline = resultImage = black_init;
//    try
//    {
		originImage.display();
		cout << "Press enter to continue:";
		cin.get();
        chkarr=doColorOptimize();
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
		doOutlineDraw(chkarr);
		manDrawOutline.display();


        if(!doOpencvAnalyse())
        {
            //if(!doFinalAnalyse())
            {

                cout << "Image is refused to be recoginized, try another picture." << endl;
                return 0;
            }
        }
        cout << "Result:beginx=" << analyseResult.bottomLine.lineBegin.x << "beginy=" << analyseResult.bottomLine.lineBegin.y << endl;
        cout << "Result:endx=" << analyseResult.bottomLine.lineEnd.x << "endy=" << analyseResult.bottomLine.lineEnd.y << endl;
        putHatOn();
//    }
    originImage.display();
    if(outputFileName.empty())
        outputFileName = "output.png";
    originImage.save_png(outputFileName.c_str());
    cout << "Done. Result saved to " << outputFileName << " successfully." << endl;



    return 0;
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
    auto strLeftEqual = [](const string &sa, const string &sb) -> bool {
        if(sa.size() <= sb.size())
            return false;
        for(size_t cter = 0; cter < sb.size(); ++cter)
        {
            if(sa[cter]!=sb[cter])
                return false;
        }
        return true;
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
        checkedResult = readCfgValueF(cfgValBuf, "blur_number");
        if(checkedResult.first)
        {
            blur_number = checkedResult.second;
            continue;
        }
        if(strLeftEqual(cfgValBuf, "lefthat="))
        {
            hatImageInfos.first.push_back(cfgValBuf.substr(8));
            continue;
        }
        if(strLeftEqual(cfgValBuf, "righthat="))
        {
            hatImageInfos.second.push_back(cfgValBuf.substr(9));
            continue;
        }
    }
    return;
}
