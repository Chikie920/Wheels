#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>

void load(void);
void updatewithinput(void);
void updatewithoutinput(void);
void show(void);

int width = 640, height = 480;
int plane_x, plane_y, battle_x, battle_y, enemy_x, enemy_y;
char ch;
int battle, enemy;
int plane_speed, battle_speed, enemy_speed;
int goals;

int main(void)
{   
    initgraph(width,height);
    setbkcolor(WHITE);
    setcaption("Play Plane");
    load();
    delay_fps(60);
    while(1){
        cleardevice();
        updatewithinput();
        updatewithoutinput();
        show();
        Sleep(10);
    }

    return 0;
}

void load(void)
{
    plane_x = 320;
    plane_y = 240;
    plane_speed = 40;
    battle = 0;
    battle_speed = 40;
    goals = 0;
    enemy_speed = 20;
    enemy = 1;

    enemy_x = rand()%620;
    enemy_y = rand()%240;
}

void updatewithinput(void)
{
    if(kbhit()){
        ch = getch();
        switch(ch){
            case 'A':
            case 'a': plane_x-=plane_speed;
            break;
            case 'D':
            case 'd': plane_x+=plane_speed;
            break;
            case 'W':
            case 'w': plane_y-=plane_speed;
            break;
            case 'S':
            case 's': plane_y+=plane_speed;
            break;
            case ' ': battle = 1;
            battle_x = plane_x;
            battle_y = plane_y - 10;
            break;
            default:
            break;
        }
    }
}

void updatewithoutinput(void)
{

    while(battle_y>0){
        battle_y -= battle_speed;
        setcolor(WHITE);
        xyprintf(battle_x,battle_y+40,"|");
        show();
        Sleep(30);
    }
    battle = 0;

    /*while(enemy_y<480){
        enemy+=enemy_speed;
        show();
        Sleep(50);
    }*/

}

void show(void)
{
    /*setcolor(WHITE);
    xyprintf(enemy_x,enemy_y,"$");*/
    setcolor(WHITE);
    xyprintf(plane_x,plane_y,"@");
    setcolor(BLACK);
    xyprintf(560,20,"%.2fFPS",getfps());
    xyprintf(20,20,"Your Goals are %d",goals);

    if(battle){
        xyprintf(battle_x,battle_y,"|");
        updatewithinput();
    }

    setcolor(RED);
    xyprintf(plane_x,plane_y,"@");
    /*setcolor(BLACK);
    xyprintf(enemy_x,enemy_y,"$");*/

}