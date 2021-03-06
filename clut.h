//#ifndef CLUT_H
//#define CLUT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>

#define SIZECOLOR 7
#define NBCOLOR 200
#define WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

typedef struct Color Color;
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    int cub;
    int indexColor;
};

typedef struct Point3d Point3d;
struct Point3d {
  int x;
  int y;
  int z;
  int alive;
  Color color;
};

typedef struct site_t site_t;
struct site_t {
  int x, y;
  Color color;
  int alive;
};

typedef struct recurr recurr;
struct recurr {
  int nombre;
  int stock;
};

typedef struct myGlubyte myGlubyte;
struct myGlubyte {
    GLubyte r,g,b;
};


static int radius;
static int cube ;
static Color ma_clut[SIZECOLOR][SIZECOLOR][SIZECOLOR];
static Point3d ClutIndexColor[NBCOLOR];

static recurr recurrence[1024];
static int visited[NBCOLOR][NBCOLOR][NBCOLOR];

//#endif
