#include <graphics.h>
#include <stdio.h>
#define NAME "Snake"
#define RADIUS 2

int main(void)
{
    initgraph(640,480,NULL);
    setcaption(NAME);
    setbkcolor(EGERGB(225,225,225),NULL);//创建游戏界面

    

    getch();
    closegraph();

    return 0;
}