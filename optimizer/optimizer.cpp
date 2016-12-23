#include "optimizer.hpp"

using namespace cimg_library;

std::vector<std::vector<bool> > doColorOptimize(CImg<unsigned char> &originImage,CImg<unsigned char> &colorOptimizedImage)
{
    colorOptimizedImage=originImage;
    std::queue<std::pair<int,int> > bfs_queue;
    std::vector<readyToChange>change_queue;
    colorOptimizedImage.blur(blur_number);
    int size_x=originImage.width(),size_y=originImage.height();
    unsigned long long r_aver=0,g_aver=0,b_aver=0;
    std::vector<std::vector<bool> > visited,unusable;
    for(int i=0;i<size_x;++i)
    {
        std::vector<bool> tmpvec;
        for(int j=0;j<size_y;++j)
            tmpvec.push_back(false);
        visited.push_back(tmpvec);
        unusable.push_back(tmpvec);
    }
    cimg_forXY(originImage,x,y)
    {
        if(!visited[x][y])
        {
            r_aver=0,g_aver=0,b_aver=0;
            change_queue.clear();
            while(!bfs_queue.empty())
                bfs_queue.pop();
            bfs_queue.push(std::make_pair(x,y));
            cimg_color begin_color(originImage.atXYZC(x,y,0,0),originImage.atXYZC(x,y,0,1),originImage.atXYZC(x,y,0,2));
            change_queue.push_back(readyToChange(x,y,begin_color));
            r_aver+=begin_color[0],g_aver+=begin_color[1],b_aver+=begin_color[2];
            visited[x][y]=true;
            int cnt=0;
            while(!bfs_queue.empty())
            {
                std::pair<int,int> top_element=bfs_queue.front();
                bfs_queue.pop();
                int now_x=top_element.first,now_y=top_element.second;
                cimg_color now(originImage.atXYZC(now_x,now_y,0,0),originImage.atXYZC(now_x,now_y,0,1),originImage.atXYZC(now_x,now_y,0,2));
                for(int direct=0;direct<8;++direct)
                {
                    int next_x=now_x+dx[direct],next_y=now_y+dy[direct];
                    if(next_x<0||next_x>=size_x||next_y<0||next_y>=size_y||visited[next_x][next_y])
                        continue;
                    cimg_color next(originImage.atXYZC(next_x,next_y,0,0),originImage.atXYZC(next_x,next_y,0,1),originImage.atXYZC(next_x,next_y,0,2));
                    if(next-now<eps)
                    {
                        bfs_queue.push(std::make_pair(next_x,next_y));
                        change_queue.push_back(readyToChange(next_x,next_y,next));
                        r_aver+=next[0],g_aver+=next[1],b_aver+=next[2];
                        visited[next_x][next_y]=true;
                    }
                }
            }
            r_aver/=change_queue.size(),g_aver/=change_queue.size(),b_aver/=change_queue.size();
            if(change_queue.size()<face_size*size_x*size_y)
            {
                for(std::vector<readyToChange>::iterator iter=change_queue.begin();iter!=change_queue.end();++iter)
                {
                    int nx=(*iter).x,ny=(*iter).y;
                    colorOptimizedImage(nx,ny,0,0)=r_aver,colorOptimizedImage(nx,ny,0,1)=g_aver,colorOptimizedImage(nx,ny,0,2)=b_aver;
                }
            }
            else
            {
                for(std::vector<readyToChange>::iterator iter=change_queue.begin();iter!=change_queue.end();++iter)
                {
                    int nx=(*iter).x,ny=(*iter).y;
                    unusable[nx][ny]=true;
                }
            }
        }
    }
    return unusable;
}
