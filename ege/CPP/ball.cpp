#include <iostream>
#include <string>
#include <graphics.h>

#define GRAPH_WIDTH 600
#define GRAPH_HEIGHT 400

using namespace std;

void InitGraph();
void Load();
void Ball();
void GetFps();

int ball_r, ball_x, ball_y, ball_speed_x, ball_speed_y;

int main()
{
    InitGraph();
    Load();
    while(1){
        Ball();
        GetFps();
        delay_fps(25);
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
    ball_r = 20;
    ball_x = GRAPH_WIDTH%400;
    ball_y = GRAPH_HEIGHT%200;
    ball_speed_x = 25;
    ball_speed_y = 15;
}

void Ball()
{
    setcolor(0, NULL);
    circle(ball_x, ball_y, ball_r);
    if(ball_x + ball_speed_x - ball_r <= 0){
        ball_x = -(ball_x + ball_speed_x - ball_r);
        ball_speed_x = -ball_speed_x;
    } else {
        ball_x += ball_speed_x;
    }

    if(ball_x + ball_speed_x + ball_r >= 600){
        ball_x = GRAPH_WIDTH*2 - ball_x - ball_speed_x + ball_r;
        ball_speed_x = -ball_speed_x;
    } else {
        ball_x += ball_speed_x;
    }

    if(ball_y + ball_speed_y - ball_r <= 0){
        ball_y = -(ball_y + ball_speed_y - ball_r);
        ball_speed_y = -ball_speed_y;
    } else {
        ball_y += ball_speed_y;
    }

    if(ball_y + ball_speed_y + ball_r >= 400){
        ball_y = GRAPH_HEIGHT*2 - ball_y - ball_speed_y + ball_r;
        ball_speed_y = -ball_speed_y;
    } else {
        ball_y += ball_speed_y;
    }

}

void GetFps()
{
    float fps = getfps();
    setcolor(0);
    xyprintf(5, 5, "FPS: %.2f", fps);
}