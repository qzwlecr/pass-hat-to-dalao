#include "../CImgDir/CImg.h"
#include <iostream>
using namespace cimg_library;
int main(int, char **args)
{
	CImg<unsigned int> img(args[1]);
	img.display();
}
