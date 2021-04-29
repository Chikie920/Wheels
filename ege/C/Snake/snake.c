#include <stdio.h>
#include <graphics.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480

void load(void);
void start(void);
void food(void);
void UpdateWithInput(void);
void UpdateWithoutInput(void);
void is_over(void);

int snake_x, snake_y, snake_len = 3, snake_long, snake_speed;
int x_move, y_move;
int last_x, last_y;
int food_x, food_y;
int snake[10][2] = {0};
char ch, last_ch;
int scores;

int main(void)
{
    initgraph(WIDTH,HEIGHT,NULL);
    setcaption("Snake");
    setbkcolor(WHITE);
    begin:cleardevice();
    snake_len = 3;
    load();
    start();
    while(1){
        delay_fps(60);
        UpdateWithInput();
        UpdateWithoutInput();
        food();
        delay(150);
        for(int n = 1;n<snake_len;++n){
            if(snake[0][0]==snake[n][0]&&snake[0][1]==snake[n][1]){
                goto game_over;
            }
        }
        if(snake[0][0]<0||snake[0][0]>640||snake[0][1]<0||snake[0][1]>480){
            game_over:;
            cleardevice();
            xyprintf(260,220,"Game Over");
            xyprintf(260,250,"Push R or r To Continue");
            if(getch()=='R'|| getch() =='r'){
            goto begin;
            } else {
                closegraph();
            }
        }
    }

    return 0;
}

void load(void)
{
    snake[0][0] = WIDTH/2;
    snake[0][1] = HEIGHT/2;
    snake_long = 15;
    snake_speed = 15;
    x_move = -15;
    y_move = 0;
    food_x = rand()%600;
    food_y = rand()%440;
    scores = 0;
}

void start(void){
    setcolor(RED);
    xyprintf(snake[0][0],snake[0][1],"*");
    for(int a = 1;a<snake_len;a++){
        setcolor(BLACK);
        snake[a][0] = snake[a-1][0] + snake_long;
        snake[a][1] = snake[a-1][1];
        xyprintf(snake[a][0],snake[a][1],"*");
    }
    setcolor(RED);
    xyprintf(food_x,food_y,"@");
}

void UpdateWithInput(void)
{
    last_x = snake[snake_len-1][0];
    last_y = snake[snake_len-1][1];
    for(int a = snake_len-1;a>0;--a){
        snake[a][0] = snake[a-1][0];
        snake[a][1] = snake[a-1][1];
    }

    if(kbhit()){
        ch = getch();
        if(last_ch != ch){
            switch(ch){
                case 'W':
                case 'w': snake[0][1]-=snake_speed;x_move = 0;y_move = -15;last_ch = 's';
                if(ch=='w'){
                } else {
                    last_ch = 'S';
                }
                break;
                case 'A':
                case 'a': snake[0][0]-=snake_speed;x_move = -15;y_move = 0;last_ch = 'd';
                if(ch=='a'){
                } else {
                    last_ch = 'D';
                }
                break;
                case 'D':
                case 'd': snake[0][0]+=snake_speed;x_move = 15;y_move = 0;last_ch = 'a';
                if(ch=='d'){
                } else {
                    last_ch = 'A';
                }
                break;
                case 'S':
                case 's': snake[0][1]+=snake_speed;x_move = 0;y_move = 15;last_ch = 'w';
                if(ch=='s'){
                } else {
                    last_ch = 'W';
                }
                default:
                break;
            } 
        } else {
            snake[0][0]+=x_move;
            snake[0][1]+=y_move;
                }
    } else {
        snake[0][0]+=x_move;
        snake[0][1]+=y_move;
    }
}

void UpdateWithoutInput(void)
{
    setcolor(BLACK);
    xyprintf(5,5,"Your score(s):%d",scores);
    setcolor(RED);
    xyprintf(snake[0][0],snake[0][1],"*");
    setcolor(WHITE);
    xyprintf(last_x,last_y,"*");
    for(int a=1;a<snake_len;a++){
        setcolor(BLACK);
        xyprintf(snake[a][0],snake[a][1],"*");
    }
}

void food(void)
{
    if((snake[0][0]>=food_x-10&&snake[0][0]<=food_x+10)&&(snake[0][1]<=food_y+10&&snake[0][1]>=food_y-10)){
        setcolor(WHITE);
        xyprintf(food_x,food_y,"@");
        food_x = rand()%600;
        food_y = rand()%440;
        setcolor(RED);
        xyprintf(food_x,food_y,"@");
        ++scores;
        if(snake_len<10){
            ++snake_len;
        }
    }
}
