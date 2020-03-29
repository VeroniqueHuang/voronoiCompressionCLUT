#include <unistd.h>
#include <math.h>
#include "ima.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "clut.h"

typedef struct Color Color;
struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    int cub;
};

typedef struct Point3d Point3d;
struct Point3d {
  int x;
  int y;
  int z;
  int alive;
  Color color;
};
static Point3d diag[NBCOLOR];

typedef struct site_t site_t;
struct site_t {
  int x, y;
  Color color;
  int alive;
};
static int radius;
static int cube ;

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

int isAllColored(){
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        if(ma_clut[i][j][k].r==444.0f && ma_clut[i][j][k].g==444.0f && ma_clut[i][j][k].b==444.0f){
          return 0;
        }
      }
    }
  }
  return 1;
}

int notColored(int x, int y, int z){
  printf("-----------------------------------[%d %d %d]%f %f %f\n",x,y,z,ma_clut[x][y][z].r,ma_clut[x][y][z].g,ma_clut[x][y][z].b);
  if(ma_clut[x][y][z].r ==444.0f && ma_clut[x][y][z].g==444.0f && ma_clut[x][y][z].b==444.0f){
    return 1;
  }
  return 0;
}

int inScreen(int x, int y, int z){
  if((x>=0 && x<SIZECOLOR) && (y>=0 && y<SIZECOLOR) && (z>=0 && z<SIZECOLOR))
    return 1;
  return 0;
}

void setColorPixel(int x, int y, int z, GLfloat r, GLfloat g, GLfloat b){
  ma_clut[x][y][z].r = (GLfloat)r;
  ma_clut[x][y][z].g = (GLfloat)g;
  ma_clut[x][y][z].b = (GLfloat)b;
  ma_clut[x][y][z].cub = radius+1;
}

int circleNOM(int x0, int y0, int z0, GLfloat r, GLfloat g, GLfloat b){
  int ret=0;

  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        if(ma_clut[i][j][k].cub == radius ){
          x0=x0+radius;
          y0=y0+radius;
          z0=z0+radius;

          printf("%d %d %d aaaaaaaaaaa clut:%d radius:%d \n", i, j ,k, ma_clut[i][j][k].cub, radius );
          if(inScreen(i+1, j, k)) {if(notColored(i+1, j, k)) {setColorPixel(i+1, j, k, r,g,b); ret=1;}}
          if(inScreen(i-1, j, k)) {if(notColored(i-1, j, k)) {setColorPixel(i-1, j, k, r,g,b); ret=1;}}

          if(inScreen(i, j+1, k)) {if(notColored(i, j+1, k)) {setColorPixel(i, j+1, k, r,g,b); ret=1;}}
          if(inScreen(i, j-1, k)) {if(notColored(i, j-1, k)) {setColorPixel(i, j-1, k, r,g,b); ret=1;}}

          if(inScreen(i, j, k+1)) {if(notColored(i, j, k+1)) {setColorPixel(i, j, k+1, r,g,b); ret=1;}}
          if(inScreen(i, j, k-1)) {if(notColored(i, j, k-1)) {setColorPixel(i, j, k-1, r,g,b); ret=1;}}
        }//end if

      }
    }
  }//end for
  return ret;
}


int diagGrow(){
  int i, fini = 1;

  for(i=0; i<NBCOLOR; i++){

    if(circleNOM(diag[i].x,diag[i].y,diag[i].z, diag[i].color.r, diag[i].color.g, diag[i].color.b)){
      fini=0;
    }
    else{
      diag[i].alive=0;
      //printf("finish voronoi number%d \n",i);
    }
  }
  radius++;
  cube++;
  return !fini;
}

void maclutInit(){
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
          ma_clut[i][j][k].r=444.0f;
          ma_clut[i][j][k].g=444.0f;
          ma_clut[i][j][k].b=444.0f;
          ma_clut[i][j][k].cub = -100;
        }
      }
    }
  }

void diagInit(){
  maclutInit();
  int i, x,y,z;
  radius = 1;
  cube = 1;
  for(i=0; i<NBCOLOR; i++){ //first : select random coordinate(x,y,z)

    x = rand_a_b(0,SIZECOLOR);
    y = rand_a_b(0,SIZECOLOR);
    z = rand_a_b(0,SIZECOLOR);




    diag[i].x = x;
    diag[i].y = y;
    diag[i].z = z;
    diag[i].alive = 1;
    diag[i].color.r = (GLfloat)diag[i].x/5;
    diag[i].color.g = (GLfloat)diag[i].y/5;
    diag[i].color.b = (GLfloat)diag[i].z/5;

    ma_clut[x][y][z] = getColor(x,y,z); //set the point color to a random position
    ma_clut[x][y][z].cub = cube;
    //printf(" color %d : (%f,%f,%f) \n", i,ma_clut[x][y][z].r,ma_clut[x][y][z].g,ma_clut[x][y][z].b);
  }
}

void affiche(){
  printf("-----------------\n");
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        printf("%f %f %f \n",ma_clut[i][j][k].r,ma_clut[i][j][k].g,ma_clut[i][j][k].b);
      }
        printf("\n");
    }
  }
}


void afficheCLUT(){
  printf("-----------------\n\n");
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        if(ma_clut[i][j][k].r == 444.0f){
          printf("0");
        }
        else{printf("X");}
      }
        printf("\n");
    }
    printf("________\n");
  }
}


void voronoi(){

  diagInit();
  while( (diagGrow() != 0) ){
    printf("again \n");
    if(radius == 4){
      printf(" radius 3\n");
      break;
    }
  }

  afficheCLUT();

  return;
}
