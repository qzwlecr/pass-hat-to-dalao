#include "stdafx.hpp"
#include <iostream>
using namespace std;
using namespace cimg_library;
void test(const unsigned char *ar)
{
    cout << ar[0] << '|' << ar[1] << '|' << ar[2];
}
int main()
{
    cimg_color cc('A','5','c');
    test(cc);
    cout << "\ndone."<< endl;
    return 0;

}
