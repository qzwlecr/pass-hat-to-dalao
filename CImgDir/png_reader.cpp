#include "../stdafx.hpp"

#include <opencv2/core.hpp>
#include <random>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <string>

CImg<unsigned int> recolic_read_png(const string &where)
{
    CImg<unsigned int> tor(where.c_str());
    return tor;
}
