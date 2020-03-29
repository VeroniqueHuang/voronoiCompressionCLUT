#include <unistd.h>
#include <math.h>
#include "ima.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include <time.h>
#include "clut.h"

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
static Point3d diag[NBCOLOR];

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
        if(ma_clut[i][j][k].r==-10.0f && ma_clut[i][j][k].g==-10.0f && ma_clut[i][j][k].b==-10.0f){
          return 0;
        }
      }
    }
  }
  return 1;
}

int notColored(int x, int y, int z){
  if(ma_clut[x][y][z].r ==-10.0f && ma_clut[x][y][z].g==-10.0f && ma_clut[x][y][z].b==-10.0f){
    return 1;
  }
  return 0;
}

int inScreen(int x, int y, int z){
  if((x>=0 && x<SIZECOLOR) && (y>=0 && y<SIZECOLOR) && (z>=0 && z<SIZECOLOR))
    return 1;
  return 0;
}

void setColorPixel(int x, int y, int z, GLfloat r, GLfloat g, GLfloat b, int nb){
  ma_clut[x][y][z].r = (GLfloat)r;
  ma_clut[x][y][z].g = (GLfloat)g;
  ma_clut[x][y][z].b = (GLfloat)b;
  ma_clut[x][y][z].cub = radius+1;
  ma_clut[x][y][z].diagnb = nb;
}

int circleNOM(int x0, int y0, int z0, GLfloat r, GLfloat g, GLfloat b, int nb){
  int ret=0;

  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        if((ma_clut[i][j][k].diagnb == nb) && (ma_clut[i][j][k].cub == radius)){
          x0=x0+radius;
          y0=y0+radius;
          z0=z0+radius;

          if(inScreen(i+1, j, k)) {if(notColored(i+1, j, k)) {setColorPixel(i+1, j, k, r,g,b,nb); ret=1;}}
          if(inScreen(i-1, j, k)) {if(notColored(i-1, j, k)) {setColorPixel(i-1, j, k, r,g,b,nb); ret=1;}}

          if(inScreen(i, j+1, k)) {if(notColored(i, j+1, k)) {setColorPixel(i, j+1, k, r,g,b,nb); ret=1;}}
          if(inScreen(i, j-1, k)) {if(notColored(i, j-1, k)) {setColorPixel(i, j-1, k, r,g,b,nb); ret=1;}}

          if(inScreen(i, j, k+1)) {if(notColored(i, j, k+1)) {setColorPixel(i, j, k+1, r,g,b,nb); ret=1;}}
          if(inScreen(i, j, k-1)) {if(notColored(i, j, k-1)) {setColorPixel(i, j, k-1, r,g,b,nb); ret=1;}}
        }//end if

      }
    }
  }//end for
  return ret;
}


int diagGrow(){
  int i, fini = 1;

  for(i=0; i<NBCOLOR; i++){

    if(circleNOM(diag[i].x,diag[i].y,diag[i].z, diag[i].color.r, diag[i].color.g, diag[i].color.b, i)){
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
          ma_clut[i][j][k].r=-10.0f;
          ma_clut[i][j][k].g=-10.0f;
          ma_clut[i][j][k].b=-10.0f;
          ma_clut[i][j][k].cub = -10;
        }
      }
    }
  }

int doublon(int x, int y, int z){
  for(int i=0; i<NBCOLOR; i++){
    if(doublons[i].x==x && doublons[i].y==y && doublons[i].z==z)
      return 1;
  }
  return 0;
}

void diagInit(){
  maclutInit();
  int i,x,y,z;
  radius = 1;
  cube = 1;
  for(i=0; i<NBCOLOR; i++){ //first : select random coordinate(x,y,z)

    do{
      x = rand_a_b(0,SIZECOLOR);
      y = rand_a_b(0,SIZECOLOR);
      z = rand_a_b(0,SIZECOLOR);

    }while(doublon(x,y,z));

    printf("%d %d %d \n", x,y,z);
    doublons[i].x=x;
    doublons[i].y=y;
    doublons[i].z=z;

    diag[i].x = x;
    diag[i].y = y;
    diag[i].z = z;
    diag[i].alive = 1;
    diag[i].color.r = (GLfloat)diag[i].x/SIZECOLOR;
    diag[i].color.g = (GLfloat)diag[i].y/SIZECOLOR;
    diag[i].color.b = (GLfloat)diag[i].z/SIZECOLOR;

    //if(i==0){couleura.r = diag[i].color.r; couleura.g = diag[i].color.g; couleura.b = diag[i].color.b;}
    //if(i==1){couleurb.r = diag[i].color.r; couleurb.g = diag[i].color.g; couleurb.b = diag[i].color.b;}

    ma_clut[x][y][z] = getColor(x,y,z); //set the point color to a random position
    ma_clut[x][y][z].cub = cube;
    ma_clut[x][y][z].diagnb = i;

  }
}

void afficheCLUT(){
  printf("-----------------\n\n");
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        if(ma_clut[i][j][k].r == couleura.r && ma_clut[i][j][k].g == couleura.g && ma_clut[i][j][k].b == couleura.b){
          printf("0");
        }
        else if(ma_clut[i][j][k].r == couleurb.r && ma_clut[i][j][k].g == couleurb.g && ma_clut[i][j][k].b == couleurb.b){
          printf("O");
        }
        else{printf("X");}
      }
        printf("\n");
    }
    printf("________\n");
  }
}


void voronoi(){
  srand(time(NULL));

  diagInit();
  while( diagGrow() != 0 ){
  }
  printf("Clut END\n");

  //afficheCLUT();

  return;
}
