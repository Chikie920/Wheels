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
char img[15] = {"resource\\a.jpg"};
int rl = 1;

int main(void)
{
    initgraph(800,500);
    setbkcolor(WHITE);
    setcaption("CXK PLAYS BASKETBALL");
    load();
    while(1){
        delay_fps(60);
        show();
        UpdateWithInput();
        UpdateWithoutInput();
        if(ball_y>800){
            break;
        }
    }
    cleardevice();
    outtextxy(300,200,"game over");
    getch();

    return 0;
}

void load(void)
{
    pjpg = newimage(800,500);
    temp = newimage();
    ball = newimage(60,60);
    getimage(temp,"C:\\Users\\Mirai\\Desktop\\Work\\Wheels\\resource\\background.jpg");
    pjpg_width = getwidth(temp);
    pjpg_height = getheight(temp); //装载背景图像
    putimage(pjpg,0,0,800,500,temp,0,0,pjpg_width,pjpg_height);
    delimage(temp);
    temp = NULL;
    putimage(0,0,pjpg); //显示背景图像
    ball_x = rand()%720;
    ball_y = 20;
    speed_ball = 40;
    speed_cxk = 80;
    cxk_x = 350;
    cxk_y = 360;
}

void show(void)
{
    pjpg = newimage(800,500);
    temp = newimage();
    ball = newimage(60,60);
    getimage(temp,"C:\\Users\\Mirai\\Desktop\\Work\\Wheels\\resource\\background.jpg");
    putimage(pjpg,0,0,800,500,temp,0,0,pjpg_width,pjpg_height);
    delimage(temp);
    temp = NULL;
    putimage(0,0,pjpg); //显示背景图像
    getimage(ball,"C:\\Users\\Mirai\\Desktop\\Work\\Wheels\\resource\\ball.png");
    putimage(ball_x,ball_y,ball);

    if(rl){
        CXK = newimage(50,50);
        for(int i='a';i<=('a'+1);i++){
            delay_fps(100);
            *(img+9) = (char)i;
            getimage(CXK,img);
            putimage(cxk_x,cxk_y,CXK);
            delay_ms(60);
        }
    } else {
        CXK = newimage(50,50);
        for(int i='c';i<=('c'+1);i++){
            delay_fps(100);
            *(img+9) = (char)i;
            getimage(CXK,img);
            putimage(cxk_x,cxk_y,CXK);
            delay_ms(60);
        }
    } //显示CXK

}

void UpdateWithInput(void)
{
    if(kbhit()){
        ch = getch();
        switch(ch){
            case 'A':
            case 'a': cxk_x-=speed_cxk;rl = 0;
            break;
            case 'D':
            case 'd': cxk_x+=speed_cxk;rl = 1;
            break;
            default:
            break;
        }
    }
}

void UpdateWithoutInput(void)
{
    ball_y+=speed_ball;
    if(ball_y<=cxk_y&&ball_x>=cxk_x&&ball_x<=(cxk_x+getwidth(CXK))){
        speed_ball = -speed_ball;
    } else if(ball_y<=0){
        ball_x = rand()%750;
        ball_y = 20;
        speed_ball = 40;
    } else{
    }
}
