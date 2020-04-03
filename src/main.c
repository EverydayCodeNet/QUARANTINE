#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tice.h>
#include <keypadc.h>
#include <graphx.h>



typedef struct {
    int x;
    int y;
    //radius
    int i;
    int dir;
    int color;
    int speed;
    int recoveryTime;

    //infected = 1, healthy = 0, recovered = 2, going to DIE = 3
    int infected;
    int dead;

} dot;

// maxDots = # of dots - 1, arr[size]
int CheckDistance(int cnt, int arrayNum);
void ChangeStatus();
void ChangeDirection();

int maxDots = 24;
struct dot arr_allDots[25];

void CreateDots() {
    int cnt;

    for (cnt=0; cnt<= maxDots; cnt++) 
    {
    arr_allDots[cnt].x = randInt(0, 312);
    arr_allDots[cnt].y = randInt(0, 232);
    arr_allDots[cnt].i = 4;
    arr_allDots[cnt].dir = randInt(1,8);
    arr_allDots[cnt].speed = randInt(4,8);

    if (cnt == 1) {
        arr_allDots[cnt].color = 0xE0;
        arr_allDots[cnt].recoveryTime = randInt(140,210);
        arr_allDots[cnt].infected = 1;
    } else {
        arr_allDots[cnt].color = 0x1aef6f;
        arr_allDots[cnt].recoveryTime = -1000;
        arr_allDots[cnt].infected = 0;
    }
    
    gfx_SetColor(arr_allDots[cnt].color);
    gfx_FillCircle(arr_allDots[cnt].x, arr_allDots[cnt].y,arr_allDots[cnt].i);
    }
}

void CheckWalls() {
    int cnt;
    //change direction
    for (cnt=0; cnt <= maxDots; cnt++) {
        if (arr_allDots[cnt].x < 7) {
            
            if (arr_allDots[cnt].dir == 3) {
                arr_allDots[cnt].dir = 1;
            } else if(arr_allDots[cnt].dir == 7) {
                arr_allDots[cnt].dir = 5;
            } else if(arr_allDots[cnt].dir == 4) {
                arr_allDots[cnt].dir = 2;
            }
        }
        if (arr_allDots[cnt].x > 312) {
            if (arr_allDots[cnt].dir == 1)
            {
                arr_allDots[cnt].dir = 3;
            } else if (arr_allDots[cnt].dir == 2) {
                arr_allDots[cnt].dir = 4;
            } else if (arr_allDots[cnt].dir == 5) {
                arr_allDots[cnt].dir = 7;
            }
        }
        
        if (arr_allDots[cnt].y < 7) {
            if (arr_allDots[cnt].dir == 2) {
                arr_allDots[cnt].dir = 1;
            } else if (arr_allDots[cnt].dir == 8) {
                arr_allDots[cnt].dir = 6;
            } else if (arr_allDots[cnt].dir == 4) {
                arr_allDots[cnt].dir = 3;
            }
        }
        if (arr_allDots[cnt].y > 232) {
            if (arr_allDots[cnt].dir == 1) {
                arr_allDots[cnt].dir = 2;
            } else if (arr_allDots[cnt].dir == 6){
                arr_allDots[cnt].dir = 8;
            } else if (arr_allDots[cnt].dir == 3) {
                arr_allDots[cnt].dir = 4;
            }
        }
    }
}

void MoveDots() {
    int cnt;
    for (cnt=0; cnt<=maxDots; cnt++) {
        if (arr_allDots[cnt].dir == 1){
            arr_allDots[cnt].x  = arr_allDots[cnt].x + arr_allDots[cnt].speed;
            arr_allDots[cnt].y = arr_allDots[cnt].y + arr_allDots[cnt].speed;
        }else if (arr_allDots[cnt].dir == 2){
            arr_allDots[cnt].x = arr_allDots[cnt].x + arr_allDots[cnt].speed;
            arr_allDots[cnt].y = arr_allDots[cnt].y - arr_allDots[cnt].speed;
        }else if (arr_allDots[cnt].dir == 3){
            arr_allDots[cnt].x = arr_allDots[cnt].x- arr_allDots[cnt].speed;
            arr_allDots[cnt].y = arr_allDots[cnt].y+ arr_allDots[cnt].speed;
        }else if (arr_allDots[cnt].dir == 4){
            arr_allDots[cnt].x = arr_allDots[cnt].x- arr_allDots[cnt].speed;
            arr_allDots[cnt].y = arr_allDots[cnt].y- arr_allDots[cnt].speed;
        } else if (arr_allDots[cnt].dir == 5){
            arr_allDots[cnt].x = arr_allDots[cnt].x + arr_allDots[cnt].speed;
            arr_allDots[cnt].y = arr_allDots[cnt].y;
        }else if (arr_allDots[cnt].dir == 6){
            arr_allDots[cnt].x = arr_allDots[cnt].x;
            arr_allDots[cnt].y = arr_allDots[cnt].y + arr_allDots[cnt].speed;
        }else if (arr_allDots[cnt].dir == 7){
            arr_allDots[cnt].x = arr_allDots[cnt].x - arr_allDots[cnt].speed;
            arr_allDots[cnt].y = arr_allDots[cnt].y;
        }else if (arr_allDots[cnt].dir == 8){
            arr_allDots[cnt].x = arr_allDots[cnt].x;
            arr_allDots[cnt].y = arr_allDots[cnt].y - arr_allDots[cnt].speed;
        }
    }
}

void DrawDots() {
    int cnt;
    
    for (cnt=0; cnt<=maxDots; cnt++) 
    {
    
    gfx_SetColor(arr_allDots[cnt].color);
    gfx_FillCircle(arr_allDots[cnt].x, arr_allDots[cnt].y,arr_allDots[cnt].i);

    }
    gfx_SetDrawBuffer();
}

void ClearDots() {
    int cnt;
    
    for (cnt=0; cnt<=maxDots; cnt++) 
    {
    //Collision Detection
    //gfx_SetColor(255);
    gfx_FillScreen(255);
    //gfx_FillCircle_NoClip(arr_allDots[cnt].x, arr_allDots[cnt].y,arr_allDots[cnt].i);
    }
}

void DispStats(int numLoops) {
    int sick = 0;
    int healthy = 0;
    int recovered = 0;
    int cnt;
    int numDays;
    int recoveryTime;
    int dead = 0;
    //recoveryTime = arr_allDots[1].recoveryTime;
    //int arrayNum = 15;
    //int distance = CheckDistance(cnt,arrayNum);
    

    for (cnt=0; cnt<=maxDots; cnt++) {
        if (arr_allDots[cnt].infected == 1) {
            sick++;
        } else if(arr_allDots[cnt].infected == 0) {
            healthy++;
        } else if (arr_allDots[cnt].infected == 2) {
            recovered++;
        } else if (arr_allDots[cnt].infected == 3) {
            dead++;
        }
    }
    gfx_SetTextScale(2.5,2.5);
    numDays = numLoops/30 + 1;
    gfx_PrintStringXY("Day ",10,10);
    if (numDays > 10) {
    gfx_PrintInt(numDays,2);
    } else {
        gfx_PrintInt(numDays,1);
    }
    gfx_SetTextScale(1.75,1.75);
    gfx_PrintStringXY("Healthy: ",10,30);
    gfx_PrintInt(healthy,1);
    gfx_PrintStringXY("Sick: ",10,40);
    gfx_PrintInt(sick,1);
    gfx_PrintStringXY("Recovered: ",10,50);
    gfx_PrintInt(recovered,1);
    gfx_PrintStringXY("Dead: ",10,60);
    gfx_PrintInt(dead,1);
}

void CheckPos() {
    int cnt;
    int arrayNum = 1;
    int distance;
    for (cnt=0; cnt<=maxDots; cnt++) {
        for (arrayNum=0;arrayNum <= maxDots;arrayNum++) {
            
            distance = CheckDistance(cnt,arrayNum);
            if (distance < 13) {
                ChangeStatus(cnt,arrayNum);
                //ChangeDirection(cnt);
            }
            
        }
    }
}

int CheckDistance(int cnt, int arrayNum) {
    int x1 = arr_allDots[cnt].x;
    int y1 = arr_allDots[cnt].y;
    int x2 = arr_allDots[arrayNum].x;
    int y2 = arr_allDots[arrayNum].y;
    int xsqr;
    int ysqr;
    int distance;

    xsqr = (x2-x1) * (x2-x1);
    ysqr= (y2-y1) * (y2-y1);

    //distance formula
    distance = (sqrt(xsqr + ysqr));
    return distance;
}

void ChangeDirection(int cnt) {
    //current direction of dot 1 and dot 2
    int cDir1;

    cDir1 = arr_allDots[cnt].dir;

}

void CheckStatus() {
    int cnt;
    int recoveryTime;
    for (cnt=0; cnt<=maxDots; cnt++) {
    //recoveryTime = arr_allDots[cnt].recoveryTime;
    
    
    if (arr_allDots[cnt].infected == 1) {
        arr_allDots[cnt].color = 0xE0;
        arr_allDots[cnt].recoveryTime = arr_allDots[cnt].recoveryTime - 1;
    }

    if (arr_allDots[cnt].recoveryTime == 0 && arr_allDots[cnt].dead == 0) {
        arr_allDots[cnt].infected = 2;
        arr_allDots[cnt].color = 22;
        
    } else if (arr_allDots[cnt].recoveryTime == 0 && arr_allDots[cnt].dead == 1) {
        arr_allDots[cnt].color = 0;
        arr_allDots[cnt].dir = 0;
        arr_allDots[cnt].dead = 3;
        arr_allDots[cnt].infected = 3;
    }
    }
}

void ChangeStatus(int cnt, int arrayNum) {
    int deathRate = randInt(1,10);
    if (arr_allDots[cnt].infected == 1 && arr_allDots[arrayNum].infected == 0) {
        arr_allDots[arrayNum].infected = 1;
        arr_allDots[arrayNum].recoveryTime = (140,210);
        if (deathRate < 3) {
            arr_allDots[arrayNum].dead = 1;
        }

    } else if (arr_allDots[arrayNum].infected == 0 && arr_allDots[cnt].infected == 1) {
        arr_allDots[cnt].infected = 1;
        arr_allDots[cnt].recoveryTime = randInt(140,210);
        if (deathRate < 3) {
            arr_allDots[arrayNum].dead = 1;
        }
    }

}

void PrintCentered(const char *str);
int FindCenter(const char *str);

void MainMenu(int menuPos, kb_key_t key) {
    
}

void main(void) {
    kb_key_t key;  
    int numLoops = 0;
    int menuPos = 1;
    int xCenter;
    gfx_Begin();
    

    srand(rtc_Time());
    CreateDots();
    ClearDots();

    //game loop
    do {
        numLoops = numLoops++;
        CheckPos();
        CheckWalls();
        CheckStatus();
        MoveDots();
        ClearDots();
        DrawDots();
        DispStats(numLoops);
        gfx_SwapDraw();
        
    } while (!os_GetCSC());
    
    while (!os_GetCSC());
    
    gfx_End();
}

void PrintCentered(const char *str)
{
    gfx_PrintStringXY(str,
                      (LCD_WIDTH - gfx_GetStringWidth(str)) / 2,
                      (LCD_HEIGHT - 8) / 2);
}

int FindCenter(const char *str) {
    int xCenter;
    xCenter = (LCD_WIDTH - gfx_GetStringWidth(str)) / 2;
    return xCenter;
}
