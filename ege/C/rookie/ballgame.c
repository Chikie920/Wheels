#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>

void updateWithInput(void);
void updateWithoutInput(void);
void show(void);

int ball_x=50, ball_y=10;
int board_x=50, board_y=400;
int SPEED = 40;
int goals = 0;
char ch;
char pr[] = "Your goals is ";

int main(void)
{
    initgraph(640,480,NULL);
    setbkcolor(WHITE);
    delay_fps(100);
    while(1){
        cleardevice();
        show();
        updateWithoutInput();
        updateWithInput();
        Sleep(200);
    }

    return 0;
}

void updateWithInput(void)
{
    if(kbhit()){
        ch = getch();
        switch(ch){
            case 'A':
            case 'a': board_x-=80;
                break;
            case 'D':
            case 'd': board_x+=80;
                break;
            default :
                break;
            }
    }
}

void updateWithoutInput(void)
{
    ball_y+=SPEED;
    if(ball_y+40>=board_y&&ball_x>=board_x&&ball_x<board_x+100){
        SPEED=-SPEED;
        goals++;
    } else if(ball_y>640){
        cleardevice();
        xyprintf(270,240,"Game Over");
    } else if(ball_y<=0){
        ball_x = rand()%460;
        ball_y = rand()%20;
        SPEED = - SPEED;
    }

}

void show(void)
{
    setcolor(BLACK);
    circle(ball_x,ball_y,10);
    line(board_x,board_y,board_x+100,board_y);
    pr[14]=5;
    xyprintf(10,430,"Your goal is %d",goals);
}