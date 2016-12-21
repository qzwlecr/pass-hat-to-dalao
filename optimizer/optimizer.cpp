#include "optimizer.hpp"

using namespace cimg_library;

void doColorOptimize(CImg<unsigned char> &originImage,CImg<unsigned char> &colorOptimizedImage)
{
    std::queue<std::pair<int,int> > bfs_queue;
    std::vector<readyToChange>change_queue;
    int size_x=originImage.width(),size_y=originImage.height();
    bool **visited=new bool*[size_y];
    for(int i=0;i<size_y;i++)
        visited[i]=new bool[size_x];
    memset(visited,false,sizeof(visited));
    cimg_forXY(originImage,x,y)
    {
        if(!visited[x][y])
        {
            change_queue.clear();
            bfs_queue.push(std::make_pair(x,y));
            cimg_color begin_color(originImage.atXYZC(x,y,0,0),originImage.atXYZC(x,y,0,1),originImage.atXYZC(x,y,0,2));
            change_queue.push_back(readyToChange(x,y,begin_color));
            while(!bfs_queue.empty())
            {
                bool push_flag=false;
                std::pair<int,int> top_element=bfs_queue.front();
                bfs_queue.pop();
                int now_x=top_element.first,now_y=top_element.second;
                cimg_color now(originImage.atXYZC(now_x,now_y,0,0),originImage.atXYZC(now_x,now_y,0,1),originImage.atXYZC(now_x,now_y,0,2));
                for(int x_r=0;x_r<8;++x_r)
                    for(int y_r=0;y_r<8;++y_r)
                    {
                        int next_x=now_x+dx[x_r],next_y=now_y+dy[y_r];
                        if(next_x<0||next_x>size_x||next_y<0||next_y>size_y||visited[next_x][next_y])
                            continue;
                        cimg_color next(originImage.atXYZC(next_x,next_y,0,0),originImage.atXYZC(next_x,next_y,0,1),originImage.atXYZC(next_x,next_y,0,2));
                        if(next-now<eps)
                        {
                            bfs_queue.push(std::make_pair(x,y));
                            change_queue.push_back(readyToChange(next_x,next_y,next));
                        }
                    }
            }
            unsigned long long r_aver=0,g_aver=0,b_aver=0;
            for(std::vector<readyToChange>::iterator iter=change_queue.begin();iter!=change_queue.end();++iter)
                r_aver+=(*iter).data[0],g_aver+=(*iter).data[1],b_aver+=(*iter).data[2];
            r_aver/=change_queue.size(),g_aver/=change_queue.size(),b_aver/=change_queue.size();
            for(std::vector<readyToChange>::iterator iter=change_queue.begin();iter!=change_queue.end();++iter)
            {
                int x=(*iter).x,y=(*iter).y;
                colorOptimizedImage(x,y,0,0)=r_aver,colorOptimizedImage(x,y,0,1)=g_aver,colorOptimizedImage(x,y,0,2)=b_aver;
            }
        }
    }
}
