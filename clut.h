//#ifndef CLUT_H
//#define CLUT_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>


#define SIZECOLOR 100
#define NBCOLOR 100 
#define WIDTH glutGet(GLUT_WINDOW_WIDTH)
#define HEIGHT glutGet(GLUT_WINDOW_HEIGHT)

typedef struct Color Color;
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    int cub;
    int diagnb;
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

static int radius;
static int cube ;
static Color couleura;
static Color couleurb;

static Color ma_clut[SIZECOLOR][SIZECOLOR][SIZECOLOR];
static Point3d doublons[NBCOLOR];
static Point3d diag[NBCOLOR];


//#endif
