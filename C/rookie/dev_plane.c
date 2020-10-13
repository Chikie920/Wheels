#include <stdio.h>
#include <graphics.h>
#include <stdbool.h>
#include <stdlib.h>

int x_pos = 230;
int y_pos = 240;
int speed = 50;
char ch;
bool is_fire = false;
char battle[] = "|";
struct bkb {
    int xpos;
    int ypos;
    bool fire;
};
int enemy_x, enemy_y;

int enemy(int , int );
struct bkb getkb(void);

int main(void)
{
    initgraph(640,480,NULL);
    setbkcolor(WHITE);

    circle(x_pos,y_pos,10);

     while(ch=getch()){
        delay_fps(60);
        switch(ch){
            case 'a': x_pos+=-speed;
            break;
            case 'd': x_pos+=speed;
            break;
            case 'w': y_pos+=-speed;
            break;
            case 's': y_pos+=speed;
            break;
            case ' ': is_fire=true;
        }
        cleardevice();
        circle(x_pos,y_pos,10);

        int battle_x = x_pos;
        int battle_y = y_pos-5;

        while(is_fire){
            while(battle_y>=0){
                battle_y+=-speed;
                int re = enemy(battle_x, battle_y);
                if(re){
                    battle_y = -1;
                }
                circle(x_pos,y_pos,10);
                outtextxy(battle_x,battle_y,battle);
                struct bkb backbk = getkb();
                x_pos=backbk.xpos;
                y_pos=backbk.ypos;
                cleardevice();
            }
            is_fire=false;
        }
     }

    return 0;
}

struct bkb getkb(void)
{
 if(ch=getch()){
        switch(ch){
            case 'a': x_pos+=-speed;
            break;
            case 'd': x_pos+=speed;
            break;
            case 'w': y_pos+=-speed;
            break;
            case 's': y_pos+=speed;
            break;
            case ' ': is_fire=true;
        }
    }
    struct bkb bkbs;
    bkbs.xpos=x_pos;
    bkbs.ypos=y_pos;

    return (bkbs);
}

int enemy(int battle_x, int battle_y)
{
    enemy_x = 20;
    enemy_y = 20;
    while(enemy_y){
        setcolor(RED);
        circle(enemy_x,enemy_y,10);
        if(enemy_y==battle_y && enemy_x==battle_x){
            return 1;
        }
    }
}