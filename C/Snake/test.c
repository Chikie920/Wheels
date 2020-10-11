#include <stdio.h>
#include <graphics.h>
#define NAME "Snake Game"
#define SPEED 50

int main(void){
    int x , y;
    int x1 = 320;
    int x2 = 380;
    int y1 , y2;
    y1 = y2 = 240;
    mouse_msg msg;

    initgraph(640,480,NULL);
    setcaption(NAME);
    setbkcolor(EGERGB(225,225,225),NULL);
    outtextxy(315,240,"Snake Game");
    outtextxy(318,260,"Begin");
 //   outtextxy(316,280,"History");
    while(mousemsg()){
        msg = getmouse();
    /*    if(msg.mkLButton==WM_LBUTTONDOWN){
            mousepos(&x,&y);
        }
        else{
            continue;
        }*/
    }

    if(x>=318&&x<=323&&260<y&&y<265){
        cleardevice();
        setcolor(BLACK);
    setlinewidth(2,NULL);
    line(x1,y1,x2,y2);

    while(x1>0){
        x1=x1-SPEED;
        x2=x2-SPEED;
        printf("%d ",x1);
        delay(500);
        delay_fps(60);
        cleardevice();
        setcolor(BLACK);
        line(x1,y1,x2,y2);
    }

    if(x1<0){
        cleardevice();
        outtextxy(300,240,"Game Over");
        getch();
        closegraph();
    }
    }

    return 0;
}