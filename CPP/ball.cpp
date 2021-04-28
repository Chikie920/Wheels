#include <iostream>
#include <graphics.h>

#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 400

using namespace std;

void InitGraph();
void Load();
void Ball();

int ball_x, ball_y, ball_speed_x, ball_speed_y;

int main()
{
    InitGraph();
    Load();
    while(1){
        Ball();
        cleardevice();
    }
    return 0;
}

void InitGraph()
{
    initgraph(GRAPH_WIDTH, GRAPH_HEIGHT, 0);
    setcaption("Ball Game");
    setbkcolor(WHITE);
    ege_enable_aa(true, NULL);
}

void Load()
{
    ball_x = GRAPH_WIDTH/2;
    ball_y = GRAPH_HEIGHT/2;
    ball_speed_x = 25;
    ball_speed_y = 15;
}

void Ball()
{
    setcolor(0, NULL);
    circle(ball_x, ball_y, 10);
    if(ball_x + ball_speed_x <= 0){
        ball_x = -(ball_x + ball_speed_x);
        ball_speed_x = -ball_speed_x;
    } else {
        ball_x += ball_speed_x;
    }

    if(ball_x + ball_speed_x >= 600){
        ball_x = GRAPH_WIDTH*2 - ball_x - ball_speed_x;
        ball_speed_x = -ball_speed_x;
    } else {
        ball_x += ball_speed_x;
    }

    if(ball_y + ball_speed_y <= 0){
        ball_y = -(ball_y + ball_speed_y);
        ball_speed_y = -ball_speed_y;
    } else {
        ball_y += ball_speed_y;
    }

    if(ball_y + ball_speed_y >= 400){
        ball_y = GRAPH_HEIGHT*2 - ball_y - ball_speed_y;
        ball_speed_y = -ball_speed_y;
    } else {
        ball_y += ball_speed_y;
    }

    Sleep(200);
}