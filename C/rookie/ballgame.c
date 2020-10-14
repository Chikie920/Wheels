#include <stdio.h>
#include <graphics.h>

void updateWithInput(void);
void updateWithoutInput(void);

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
    setcolor(BLACK);
    circle(ball_x,ball_y,10);
    line(board_x,board_y,board_x+80,board_y);
    while(1){
        updateWithoutInput();
        updateWithInput();
        pr[14]=5;
        xyprintf(10,430,"Your goal is %d",goals);
    }

    return 0;
}

void updateWithInput(void)
{
    setcolor(WHITE);
    line(board_x,board_y,board_x+80,board_y);
    if(kbhit()){
        ch = getch();
        switch(ch){
            case 'a': board_x-=40;
                break;
            case 'd': board_x+=40;
                break;
            default :
                break;
            }
    }
    Sleep(100);
    setcolor(BLACK);
    line(board_x,board_y,board_x+80,board_y);
    Sleep(100);
}

void updateWithoutInput(void)
{
    setcolor(WHITE);
    circle(ball_x,ball_y,10);
    ball_y+=SPEED;
    if(ball_y>=board_y-33&&ball_x>=board_x&&ball_x<board_x+80){
        SPEED=-SPEED;
        goals++;
    }
    Sleep(100);
    setcolor(BLACK);
    circle(ball_x,ball_y,10);
    Sleep(100);
}