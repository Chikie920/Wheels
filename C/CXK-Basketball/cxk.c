#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>

void show(void);
void UpdateWithInput(void);
void UpdateWithoutInput(void);
void load(void);

int cxk_x, cxk_y;
int ball_x, ball_y;
int speed_ball, speed_cxk;
PIMAGE pjpg, temp, ball, CXK;
int pjpg_width, pjpg_height, cxk_width, cxk_height, ball_width, ball_height;
char ch;

int main(void)
{
    initgraph(750,500);
    setbkcolor(WHITE);
    setcaption("CXK PLAYS BASKETBALL");
    delay_fps(60);
    load();
    show();
    getch();

    return 0;
}

void load(void)
{
    pjpg = newimage(750,500);
    temp = newimage();
    ball = newimage(60,60);
    getimage(temp,"‪D:\\background.jpg");
    pjpg_width = getwidth(temp);
    pjpg_height = getheight(temp); //装载背景图像
    putimage(pjpg,0,0,750,500,temp,0,0,pjpg_width,pjpg_height);
    delimage(temp);
    temp = NULL;
    putimage(0,0,pjpg); //显示背景图像
    ball_x = rand()%720;
    ball_y = 20;
    speed_ball = 40;
    speed_cxk = 80;
    cxk_x = 500;
    cxk_y = 400;
}

void show(void)
{
    CXK = newimage(80,80);
    getimage(CXK,"‪‪D:\\cxkr1.png");
    putimage(100,100,CXK);
}

void UpdateWithInput(void)
{
    if(kbhit()){
        ch = getch();
        switch(ch){
            case 'A':
            case 'a': cxk_x+=speed_cxk;
            break;
            case 'D':
            case 'd': cxk_x-=speed_cxk;
            break;
            default:
            break;
        }
    }
}

/*void UpdateWithoutInout(void)
{
    if(ball_y<750){
        ball_y+=speed_ball;
    } else if(ball_y>=cxk_y&&ball_x>=cxk_x&&ball_x<=cxk_x+80){
        speed_ball = -speed_ball;
    } else if(ball_y>750){
        cleardevice();

    } else {
        ball_x = rand()%720;
        ball_y = 20;
        speed_ball = 40;
    }
}*/
