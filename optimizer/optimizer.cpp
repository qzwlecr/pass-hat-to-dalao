#include "optimizer.hpp"

using namespace std;
using namespace cimg_library;

bool **doColorOptimize(CImg<unsigned char> &originImage,CImg<unsigned char> &colorOptimizedImage)
{
    colorOptimizedImage=originImage;
    std::queue<std::pair<int,int> > bfs_queue;
    std::vector<readyToChange>change_queue;
    int size_x=originImage.width(),size_y=originImage.height();
    bool **visited=new bool*[size_x],**unusable=new bool*[size_x];
    unsigned long long r_aver=0,g_aver=0,b_aver=0;
    for(int i=0;i<size_x;++i)
        visited[i]=new bool[size_y],unusable[i]=new bool[size_y];
    //std::cout<<233<<std::endl;
    cimg_forXY(originImage,x,y)
    {
    //    cout<<x<<' '<<y<<endl;
        if(!visited[x][y])
        {
            change_queue.clear();
    //        cout<<"queue clear"<<endl;
            bfs_queue.push(std::make_pair(x,y));
    //        cout<<"pushed"<<endl;
            cimg_color begin_color(originImage.atXYZC(x,y,0,0),originImage.atXYZC(x,y,0,1),originImage.atXYZC(x,y,0,2));
    //        cout<<"get clear"<<endl;
            change_queue.push_back(readyToChange(x,y,begin_color));
            r_aver+begin_color[0],g_aver+=begin_color[1],b_aver+=begin_color[2];
    //      cout<<"push backed"<<endl;
            visited[x][y]=true;
    //        cout<<"true"<<endl;
            while(!bfs_queue.empty())
            {
    //            cout<<"get in the loop"<<endl;
                bool push_flag=false;
                std::pair<int,int> top_element=bfs_queue.front();
                bfs_queue.pop();
                int now_x=top_element.first,now_y=top_element.second;
                cimg_color now(originImage.atXYZC(now_x,now_y,0,0),originImage.atXYZC(now_x,now_y,0,1),originImage.atXYZC(now_x,now_y,0,2));
    //            cout<<"ready to go"<<endl;
                for(int direct=0;direct<8;++direct)
                {
    //                cout<<direct<<endl;
                    int next_x=now_x+dx[direct],next_y=now_y+dy[direct];
                    if(next_x<0||next_x>=size_x||next_y<0||next_y>=size_y||visited[next_x][next_y])
                        continue;
    //                cout<<next_x<<' '<<next_y<<endl;
                    cimg_color next(originImage.atXYZC(next_x,next_y,0,0),originImage.atXYZC(next_x,next_y,0,1),originImage.atXYZC(next_x,next_y,0,2));
                    if(next-now<eps)
                    {
                        bfs_queue.push(std::make_pair(next_x,next_y));
                        change_queue.push_back(readyToChange(next_x,next_y,next));
                        r_aver+=next[0],g_aver+=next[1],b_aver+=next[2];
                        visited[next_x][next_y]=true;
    //                    cout<<"go go go"<<endl;
                    }
    //                cout<<direct<<"end"<<endl;
                }
            }
    //        cout<<"loop end"<<endl;
    //        for(std::vector<readyToChange>::iterator iter=change_queue.begin();iter!=change_queue.end();++iter)
    //            r_aver+=(*iter).data[0],g_aver+=(*iter).data[1],b_aver+=(*iter).data[2];
    //        cout<<"sum located"<<endl;
            r_aver/=change_queue.size(),g_aver/=change_queue.size(),b_aver/=change_queue.size();
    //        cout<<"average located"<<endl;
			cout << "SIZE : " << change_queue.size() << endl;
            if(change_queue.size()>=face_size*size_x*size_y)
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
            }       //        cout<<"changed end"<<endl;
        }
    }
    return unusable;
}
