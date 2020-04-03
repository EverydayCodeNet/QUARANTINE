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

} dot_t;

// maxDots = # of dots - 1, arr[size]
int checkDistance(int idx, int otherIdx);
void changeStatus();
void changeDirection();

int maxDots = 24;
dot_t dots[25];

void createDots() {
  int idx;

  for (idx = 0; idx <= maxDots; idx++) {
    dots[idx].x = randInt(0, 312);
    dots[idx].y = randInt(0, 232);
    dots[idx].i = 4;
    dots[idx].dir = randInt(1, 8);
    dots[idx].speed = randInt(4, 8);

    if (idx == 1) {
      dots[idx].color = 0xE0;
      dots[idx].recoveryTime = randInt(140, 210);
      dots[idx].infected = 1;
    } else {
      dots[idx].color = 0x1aef6f;
      dots[idx].recoveryTime = -1000;
      dots[idx].infected = 0;
    }

    gfx_SetColor(dots[idx].color);
    gfx_FillCircle(dots[idx].x, dots[idx].y, dots[idx].i);
  }
}

void checkWalls() {
  int idx;
  //change direction
  for (idx = 0; idx <= maxDots; idx++) {
    if (dots[idx].x < 7) {

      if (dots[idx].dir == 3) {
        dots[idx].dir = 1;
      } else if (dots[idx].dir == 7) {
        dots[idx].dir = 5;
      } else if (dots[idx].dir == 4) {
        dots[idx].dir = 2;
      }
    }
    if (dots[idx].x > 312) {
      if (dots[idx].dir == 1) {
        dots[idx].dir = 3;
      } else if (dots[idx].dir == 2) {
        dots[idx].dir = 4;
      } else if (dots[idx].dir == 5) {
        dots[idx].dir = 7;
      }
    }

    if (dots[idx].y < 7) {
      if (dots[idx].dir == 2) {
        dots[idx].dir = 1;
      } else if (dots[idx].dir == 8) {
        dots[idx].dir = 6;
      } else if (dots[idx].dir == 4) {
        dots[idx].dir = 3;
      }
    }
    if (dots[idx].y > 232) {
      if (dots[idx].dir == 1) {
        dots[idx].dir = 2;
      } else if (dots[idx].dir == 6) {
        dots[idx].dir = 8;
      } else if (dots[idx].dir == 3) {
        dots[idx].dir = 4;
      }
    }
  }
}

void moveDots() {
  int idx;
  for (idx = 0; idx <= maxDots; idx++) {
    if (dots[idx].dir == 1) {
      dots[idx].x = dots[idx].x + dots[idx].speed;
      dots[idx].y = dots[idx].y + dots[idx].speed;
    } else if (dots[idx].dir == 2) {
      dots[idx].x = dots[idx].x + dots[idx].speed;
      dots[idx].y = dots[idx].y - dots[idx].speed;
    } else if (dots[idx].dir == 3) {
      dots[idx].x = dots[idx].x - dots[idx].speed;
      dots[idx].y = dots[idx].y + dots[idx].speed;
    } else if (dots[idx].dir == 4) {
      dots[idx].x = dots[idx].x - dots[idx].speed;
      dots[idx].y = dots[idx].y - dots[idx].speed;
    } else if (dots[idx].dir == 5) {
      dots[idx].x = dots[idx].x + dots[idx].speed;
      dots[idx].y = dots[idx].y;
    } else if (dots[idx].dir == 6) {
      dots[idx].x = dots[idx].x;
      dots[idx].y = dots[idx].y + dots[idx].speed;
    } else if (dots[idx].dir == 7) {
      dots[idx].x = dots[idx].x - dots[idx].speed;
      dots[idx].y = dots[idx].y;
    } else if (dots[idx].dir == 8) {
      dots[idx].x = dots[idx].x;
      dots[idx].y = dots[idx].y - dots[idx].speed;
    }
  }
}

void drawDots() {
  int idx;

  for (idx = 0; idx <= maxDots; idx++) {

    gfx_SetColor(dots[idx].color);
    gfx_FillCircle(dots[idx].x, dots[idx].y, dots[idx].i);

  }
  gfx_SetDrawBuffer();
}

void clearDots() {
  int idx;

  for (idx = 0; idx <= maxDots; idx++) {
    //Collision Detection
    //gfx_SetColor(255);
    gfx_FillScreen(255);
    //gfx_FillCircle_NoClip(dots[idx].x, dots[idx].y,dots[idx].i);
  }
}

void dispStats(int numLoops) {
  int sick = 0;
  int healthy = 0;
  int recovered = 0;
  int idx;
  int numDays;
  int recoveryTime;
  int dead = 0;
  //recoveryTime = dots[1].recoveryTime;
  //int otherIdx = 15;
  //int distance = CheckDistance(idx,otherIdx);

  for (idx = 0; idx <= maxDots; idx++) {
    if (dots[idx].infected == 1) {
      sick++;
    } else if (dots[idx].infected == 0) {
      healthy++;
    } else if (dots[idx].infected == 2) {
      recovered++;
    } else if (dots[idx].infected == 3) {
      dead++;
    }
  }
  gfx_SetTextScale(2.5, 2.5);
  numDays = numLoops / 30 + 1;
  gfx_PrintStringXY("Day ", 10, 10);
  if (numDays > 10) {
    gfx_PrintInt(numDays, 2);
  } else {
    gfx_PrintInt(numDays, 1);
  }
  gfx_SetTextScale(1.75, 1.75);
  gfx_PrintStringXY("Healthy: ", 10, 30);
  gfx_PrintInt(healthy, 1);
  gfx_PrintStringXY("Sick: ", 10, 40);
  gfx_PrintInt(sick, 1);
  gfx_PrintStringXY("Recovered: ", 10, 50);
  gfx_PrintInt(recovered, 1);
  gfx_PrintStringXY("Dead: ", 10, 60);
  gfx_PrintInt(dead, 1);
}
//change status 
void checkPos() {
  int idx;
  int otherIdx = 1;
  int distance;
  for (idx = 0; idx <= maxDots; idx++) {
    for (otherIdx = idx; otherIdx <= maxDots; otherIdx++) {

      distance = checkDistance(idx, otherIdx);
      if (distance < 13) {
        changeStatus(idx, otherIdx);
        //ChangeDirection(idx);
      }
    }
  }
}

int checkDistance(int idx, int otherIdx) {
  int x1 = dots[idx].x;
  int y1 = dots[idx].y;
  int x2 = dots[otherIdx].x;
  int y2 = dots[otherIdx].y;
  int xsqr;
  int ysqr;
  int distance;

  xsqr = (x2 - x1) * (x2 - x1);
  ysqr = (y2 - y1) * (y2 - y1);

  //distance formula
  distance = (sqrt(xsqr + ysqr));
  return distance;
}

void changeDirection(int idx) {
  //current direction of dot 1 and dot 2
  int cDir1;

  cDir1 = dots[idx].dir;
}

void checkStatus() {
  int idx;
  int recoveryTime;
  for (idx = 0; idx <= maxDots; idx++) {
    //recoveryTime = dots[idx].recoveryTime;

    if (dots[idx].infected == 1) {
      dots[idx].color = 0xE0;
      dots[idx].recoveryTime = dots[idx].recoveryTime - 1;
    }

    if (dots[idx].recoveryTime == 0 && dots[idx].dead == 0) {
      dots[idx].infected = 2;
      dots[idx].color = 22;

    } else if (dots[idx].recoveryTime == 0 && dots[idx].dead == 1) {
      dots[idx].color = 0;
      dots[idx].dir = 0;
      dots[idx].dead = 3;
      dots[idx].infected = 3;
    }
  }
}

void changeStatus(int idx, int otherIdx) {
  int deathRate = randInt(1, 10);
  if (dots[idx].infected == 1 && dots[otherIdx].infected == 0) {
    dots[otherIdx].infected = 1;
    dots[otherIdx].recoveryTime = (140, 210);
    if (deathRate < 3) {
      dots[otherIdx].dead = 1;
    }

  } else if (dots[otherIdx].infected == 0 && dots[idx].infected == 1) {
    dots[idx].infected = 1;
    dots[idx].recoveryTime = randInt(140, 210);
    if (deathRate < 3) {
      dots[otherIdx].dead = 1;
    }
  }

}

void printCentered(const char * str);
int findCenter(const char * str);

void mainMenu(int menuPos, kb_key_t key) {

}

void main(void) {
  kb_key_t key;
  int numLoops = 0;
  int menuPos = 1;
  int xCenter;
  gfx_Begin();

  srand(rtc_Time());
  createDots();
  clearDots();

  //game loop
  do {
    numLoops = numLoops++;
    checkPos();
    checkWalls();
    checkStatus();
    moveDots();
    clearDots();
    drawDots();
    dispStats(numLoops);
    gfx_SwapDraw();

  } while (!os_GetCSC());

  while (!os_GetCSC());

  gfx_End();
}

int findCenter(const char *str) {
  int xCenter;
  xCenter = (LCD_WIDTH - gfx_GetStringWidth(str)) / 2;
  return xCenter;
}