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
    // pointer to the element in list
    dot_t* dot = &(dots[idx]);
    dot->x = randInt(0, 312);
    dot->y = randInt(0, 232);
    dot->i = 4;
    dot->dir = randInt(1, 8);
    dot->speed = randInt(4, 8);

    if (idx == 1) {
      dot->color = 0xE0;
      dot->recoveryTime = randInt(140, 210);
      dot->infected = 1;
    } else {
      dot->color = 0x1aef6f;
      dot->recoveryTime = -1000;
      dot->infected = 0;
    }

    gfx_SetColor(dot->color);
    gfx_FillCircle(dot->x, dot->y, dot->i);
  }
}

void checkWalls() {
  int idx;
  //change direction
  for (idx = 0; idx <= maxDots; idx++) {
    dot_t* dot = &(dots[idx]);
    if (dot->x < 7) {
      if (dot->dir == 3) {
        dot->dir = 1;
      } else if (dot->dir == 7) {
        dot->dir = 5;
      } else if (dot->dir == 4) {
        dot->dir = 2;
      }
    }
    if (dot->x > 312) {
      if (dot->dir == 1) {
        dot->dir = 3;
      } else if (dot->dir == 2) {
        dot->dir = 4;
      } else if (dot->dir == 5) {
        dot->dir = 7;
      }
    }

    if (dot->y < 7) {
      if (dot->dir == 2) {
        dot->dir = 1;
      } else if (dot->dir == 8) {
        dot->dir = 6;
      } else if (dot->dir == 4) {
        dot->dir = 3;
      }
    }
    if (dot->y > 232) {
      if (dot->dir == 1) {
        dot->dir = 2;
      } else if (dot->dir == 6) {
        dot->dir = 8;
      } else if (dot->dir == 3) {
        dot->dir = 4;
      }
    }
  }
}

void moveDots() {
  int idx;
  for (idx = 0; idx <= maxDots; idx++) {
    dot_t* dot = &(dots[idx]);
    if (dot->dir == 1) {
      dot->x = dot->x + dot->speed;
      dot->y = dot->y + dot->speed;
    } else if (dot->dir == 2) {
      dot->x = dot->x + dot->speed;
      dot->y = dot->y - dot->speed;
    } else if (dot->dir == 3) {
      dot->x = dot->x - dot->speed;
      dot->y = dot->y + dot->speed;
    } else if (dot->dir == 4) {
      dot->x = dot->x - dot->speed;
      dot->y = dot->y - dot->speed;
    } else if (dot->dir == 5) {
      dot->x = dot->x + dot->speed;
    } else if (dot->dir == 6) {
      dot->y = dot->y + dot->speed;
    } else if (dot->dir == 7) {
      dot->x = dot->x - dot->speed;
    } else if (dot->dir == 8) {
      dot->y = dot->y - dot->speed;
    }
  }
}

void drawDots() {
  int idx;

  for (idx = 0; idx <= maxDots; idx++) {
    dot_t dot = dots[idx]; // don't need a pointer since we aren't modifying
    gfx_SetColor(dot.color);
    gfx_FillCircle(dot.x, dot.y, dot.i);

  }
  gfx_SetDrawBuffer();
}

void clearDots() {
  // int idx;

  // for (idx = 0; idx <= maxDots; idx++) {
    //Collision Detection
    //gfx_SetColor(255);
    gfx_FillScreen(255);
    //gfx_FillCircle_NoClip(dot->x, dot->y,dot->i);
  // }
}

void dispStats(int numLoops) {
  int sick = 0;
  int healthy = 0;
  int recovered = 0;
  int idx;
  int numDays;
  int dead = 0;
  //recoveryTime = dots[1].recoveryTime;
  //int otherIdx = 15;
  //int distance = CheckDistance(idx,otherIdx);

  for (idx = 0; idx <= maxDots; idx++) {
    dot_t dot = dots[idx];
    if (dot.infected == 1) {
      sick++;
    } else if (dot.infected == 0) {
      healthy++;
    } else if (dot.infected == 2) {
      recovered++;
    } else if (dot.infected == 3) {
      dead++;
    }
  }
  gfx_SetTextScale(2, 2);
  numDays = numLoops / 30 + 1;
  gfx_PrintStringXY("Day ", 10, 10);
  if (numDays > 10) {
    gfx_PrintInt(numDays, 2);
  } else {
    gfx_PrintInt(numDays, 1);
  }
  gfx_SetTextScale(1, 1);
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
  dot_t dot = dots[idx];
  dot_t otherDot = dots[otherIdx];
  int x1 = dot.x;
  int y1 = dot.y;
  int x2 = otherDot.x;
  int y2 = otherDot.y;
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
  dot_t dot = dots[idx];
  int cDir1;

  cDir1 = dot.dir;
}

void checkStatus() {
  int idx;
  for (idx = 0; idx <= maxDots; idx++) {
    dot_t* dot = &(dots[idx]);
    //recoveryTime = dot->recoveryTime;

    if (dot->infected == 1) {
      dot->color = 0xE0;
      dot->recoveryTime = dot->recoveryTime - 1;
    }

    if (dot->recoveryTime == 0 && dot->dead == 0) {
      dot->infected = 2;
      dot->color = 22;

    } else if (dot->recoveryTime == 0 && dot->dead == 1) {
      dot->color = 0;
      dot->dir = 0;
      dot->dead = 3;
      dot->infected = 3;
    }
  }
}

void changeStatus(int idx, int otherIdx) {
  int deathRate = randInt(1, 10);
  dot_t* dot = &(dots[idx]);
  dot_t* otherDot = &(dots[otherIdx]);

  if (dot->infected == 1 && otherDot->infected == 0) {
    otherDot->infected = 1;
    otherDot->recoveryTime = randInt(140, 210);
    if (deathRate < 3) {
      otherDot->dead = 1;
    }

  } else if (otherDot->infected == 0 && dot->infected == 1) {
    dot->infected = 1;
    dot->recoveryTime = randInt(140, 210);
    if (deathRate < 3) {
      otherDot->dead = 1;
    }
  }

}

void printCentered(const char * str);
int findCenter(const char * str);

void mainMenu(int menuPos, kb_key_t key) {

}

void main(void) {
  int numLoops = 0;
  gfx_Begin();

  srand(rtc_Time());
  createDots();
  clearDots();

  //game loop
  do {
    numLoops++;
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