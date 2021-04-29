#include <stdio.h>
#include <graphics.h>

int main(void)
{
    initgraph(640,480,NULL);
    setbkcolor(WHITE);

    int x_pos = 10;
    int y_pos = 10;
    int speed = 20;
    int x_speed = speed;

    circle(x_pos,y_pos,10);

    while(1){
        cleardevice();
        delay_fps(60);
        circle(x_pos,y_pos,10);
        Sleep(250);
        y_pos+=speed;
        x_pos+=x_speed;
        if(y_pos>=480||y_pos<=0){
            speed = -speed;
        } else if(x_pos>=640||x_pos<=0){
            x_speed = -x_speed;
        }
    }

    return 0;
}