#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>

void load(void);
void show(void);
void update(void);

int graph_width, graph_height;
int cell[600][600];
int newcell[600][600];

int main(void)
{
    graph_width = 600;
    graph_height = 600;
    initgraph(graph_width,graph_height);
    setbkcolor(WHITE);
    setcaption("Life of Game");
    load();
    while(1){
        cleardevice();
        update();
        show();
    }
    return 0;
}

void load(void)
{
    setcolor(BLACK);
    for(int i=40;i<=600;i+=40){
        for(int n=40;n<=600;n+=40){
            cell[i][n] = rand()%2;
        }
    }  //初始化游戏
}

void update(void)
{
    for(int i=40;i<=600-40;i+=40){
        for(int n=40;n<=600-40;n+=40){
            int counts = cell[i+40][n]+cell[i][n+40]+cell[i+40][n+40]+cell[i-40][n]+cell[i][n-40]+cell[i-40][n-40];
            if(counts==3){
                newcell[i][n]=1;
            } else if(counts==2){
                newcell[i][n]=cell[i][n];
            }else{
                newcell[i][n]=0;
            }
        }
    }
}

void show(void)
{
    for(int i=40;i<=600;i+=40){
        for(int n=40;n<=600;n+=40){
            cell[i][n] = newcell[i][n];
            if(newcell[i][n]){
                setcolor(BLACK);
                xyprintf(i,n,"*");
            } else {
                setcolor(RED);
                xyprintf(i,n,"@");
            }
        }
    } 
    Sleep(500);
}