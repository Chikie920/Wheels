#include <stdio.h>
#include <graphics.h>

void updateWithInput(void);
void updateWithoutInput(void);

int ball_x=20, ball_y=50;
int board_x=50, board_y=200;
const int SPEED = 20;
char ch;

int main(void)
{
    initgraph(640,480,NULL);
    setbkcolor(WHITE);
    while(1){
        updateWithoutInput();
        updateWithInput();
    }

    return 0;
}

void updateWithInput(void)
{
    while(ch=getch()){
    switch(ch){
        case 'a': board_x-=20;
            break;
            case 'd': board_x+=20;
            break;
            default :
            break;
        }
    outtextxy(board_x,board_y,"__________");}
}

void updateWithoutInput(void)
{
    while(ball_y<=480){
        ball_y+=SPEED;
        circle(ball_x,ball_y,10);
    }
}