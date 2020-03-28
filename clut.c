#include <unistd.h>
#include <math.h>
#include "ima.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "clut.h"

typedef struct Point3d Point3d;
struct Point3d {
  int x;
  int y;
  int z;
  int alive;
};
static Point3d diag[NBCOLOR];

typedef struct Color Color;
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

typedef struct site_t site_t;
struct site_t {
  int x, y;
  Color color;
  int alive;
};
static int radius;

/*
typedef struct Clut Clut;
struct Clut {
    Color color; //keep the color by the coordinate (r,g,b) of ma_clut
};
*/
static Color ma_clut[SIZECOLOR][SIZECOLOR][SIZECOLOR];

int rand_a_b (int a, int b){
  return rand()%(b-a)+a;
}

Color getColor(int x, int y, int z){
  Color color;
  color.r =(float) x/SIZECOLOR;
  color.g =(float) y/SIZECOLOR;
  color.b =(float) z/SIZECOLOR;
  return color;
}

int inScreen(int x, int y, int z){
  if((x>0 && x<SIZECOLOR) && (y>0 && y<SIZECOLOR) && z>0 && z<SIZECOLOR))
    return 1;
  return 0;
}

int circleNOM(int x0, int y0, int z0){
  int x, y, del, incH, incO, t = M_SQRT1_2 * r + 1;
  del = 3 - (r << 1);
  incH = 6;
  incO = 10 - (r << 2);
  for(x = 0, y = r; x <= t; x++, incH += 4, incO += 4) {
    if(inScreen(x0 + x, y0 + y, z))

  }
}

void diagGrow(){
  int i, fini = 1;
  for(i=0; i<NBCOLOR; i++){
    if(!diag[i].alive) continue;
    if(circleNOM(diag[i].x,diag[i].y,diag[i].z)){
      fini=0;
    }
    else{
      diag[i].alive=0;
      printf("finish voronoi number°%d",i);
    }
  }
  radius++;
  return !fini;
}

void diagInit(){
  int i, x,y,z;
  radius = 1;
  for(i=0; i<NBCOLOR; i++){ //first : select random coordinate(x,y,z)

    x = rand_a_b(0,SIZECOLOR);
    y = rand_a_b(0,SIZECOLOR);
    z = rand_a_b(0,SIZECOLOR);

    diag[i].x = x;
    diag[i].y = y;
    diag[i].z = z;
    diag[i].alive = 1;

    ma_clut[x][y][z] = getColor(x,y,z); //set the point color to a random position
    //printf(" color %d : (%f,%f,%f) \n", i,ma_clut[x][y][z].r,ma_clut[x][y][z].g,ma_clut[x][y][z].b);
  }
}


void voronoi(){
/*  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0;k<SIZECOLOR; k++){
      }
    }
  }*/
  diagInit();

}
