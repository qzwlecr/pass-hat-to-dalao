#include "optimizer.hpp"

using namespace cimg_library;

void doColorOptimize()
{
    std::queue<std::pair<int,int> > bfs_queue;
    CImg<unsigned char> neighboor(5,5);
    int size_x=originImage.width(),size_y=originImage.height();
    bool **visited=new bool*[size_y];
    for(int i=0;i<size_y;i++)
        visited[i]=new bool[size_x];
    memset(visited,false,sizeof(visited));
    cimg_forXY(originImage,x,y)
    {
        if(!visited[x][y])
        {
            bfs_queue.push(std::make_pair(x,y));
            while(!bfs_queue.empty())
            {
                cImg_2x2(neighboor,unsigned char);
                cimg_for2x2(originImage,i,j,0,0,neighboor,unsigned char);
            }
        }
    }
}
