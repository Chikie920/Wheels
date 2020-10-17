#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>

void load(void);
void show(void);
void upwithinput(void);
void upwithoutinput(void);
void restart(void);

int graph_w, graph_h;
int bird_x, bird_y;
int bird_speed, board_speed, drop_speed, up_speed;
char ch;
float now_fps;
int goals;
int board_counts_up, board_x;
int guabi = 0;
/*
数据声明
*/

int main(void)
{
    remake:;
    load();
    initgraph(graph_w,graph_h,NULL);
    setbkcolor(WHITE);
    setcaption("Flappy Bird");
    delay_fps(150);
    /*
    界面相关
    */
    while(1){
        cleardevice();
        show();
        upwithoutinput();
        upwithinput();
        Sleep(150);

        if(bird_x==board_x&&bird_y<board_counts_up){
            break;
        } else if(bird_x==board_x&&bird_y>board_counts_up+100){
            break;
        } else if(bird_y>graph_h){
            break;
        }
    }

    cleardevice();
    xyprintf(340,220,"Game Over");
    xyprintf(340,250,"Push R To Continue");
    if(getch()=='R'|| getch()=='r'){
        goto remake;
    } else {
        closegraph();
    }
    
    Sleep(5000);

    return 0;
}

void load(void)
{
    graph_w = 800;
    graph_h = 500;
    bird_x = 60;
    bird_y = 250;
    drop_speed = 40;
    bird_speed = 0;
    board_speed = 20;
    goals = 0;
    board_x = 800;
    up_speed = 90;
    /*
    初始化数据
    */
    board_counts_up = rand()%200;
}

void show(void)
{
    now_fps = getfps();
    setcolor(BLACK);
    xyprintf(720,20,"%.2fFPS",now_fps);
    xyprintf(10,20,"Your Golas are %d",goals);
    setcolor(RED);
    outtextxy(bird_x,bird_y,"@");
    setcolor(BLACK);
    line(board_x,0,board_x,board_counts_up);
    line(board_x,board_counts_up+100,board_x,500);
}

void upwithinput(void)
{
    if(guabi){
        bird_y = board_counts_up+50;
        goals=1000;
        xyprintf(10,50,"LuBenwei Mood Open");
    }
    if(kbhit()){
        ch = getch();
        switch(ch){
            case ' ': bird_y = bird_y-90;
            break;
            case 'W':
            case 'w':
            drop_speed = 0;
            up_speed = 0;
            board_speed = 100;
            guabi = 1;
            break;
            default:
            break;
        }
    }
}

void upwithoutinput(void)
{
    bird_y+=drop_speed;
    bird_x+=bird_speed;
    board_x -= board_speed;
    if(board_x<=0){
        board_counts_up = (int)rand()%300;
        board_x = 800;
    } else if(bird_x>board_x){
        goals++;
    } else{
    }

}
