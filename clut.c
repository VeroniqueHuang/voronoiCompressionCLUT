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
  Color color;
  color.r = ma_clut[x][y][z].r;
  color.g = ma_clut[x][y][z].g;
  color.b = ma_clut[x][y][z].b;
  if(color.r ==444.0f && color.g==444.0f && color.b==444.0f){
    return 1;
  }
  return 0;
}

int inScreen(int x, int y, int z){
  if((x>0 && x<SIZECOLOR) && (y>0 && y<SIZECOLOR) && (z>0 && z<SIZECOLOR))
    return 1;
  return 0;
}

void setColorPixel(int x, int y, int z, Color color){
  printf("couleur numero : %f %f %f \n",color.r, color.g, color.b);
  ma_clut[x][y][z].r = (float)color.r;
  ma_clut[x][y][z].g = (float)color.g;
  ma_clut[x][y][z].b = (float)color.b;
}

int circleNOM(int x0, int y0, int z0, Color colori){
//  printf("daccord \n");
  int ret=0;
  int x, y, z, del, incH, incO, t = M_SQRT1_2 * radius + 1; //M_SQRT1_2 = Value of 1/racineCarre(2)
  //printf("%d ou float %f \n", ret,M_SQRT1_2);
  del = 3 - (radius << 1);
  incH = 6;
  incO = 10 - (radius << 2);
//printf("%d ooooooooo %d radius: %d\n",x,t,radius);
    for(x = 0, y = radius, z = radius; x <= t; x++, incH += 4, incO += 4) {


    //  printf("okk innnnnnnnnnn %d %d, %d %d, %d %d, %d\n",x0,x ,y0, y,z0, z,radius);
      printf("comment ???????????? %d %d %d , radius:%d\n",x0+x ,y0+y,z0+ z,radius);

      if(inScreen(x0 + x, y0 + y, z0 + z)){  printf("daccoooord\n");if(notColored(x0 + x, y0 + y, z0 + z)) {setColorPixel(x0 + x, y0 + y, z0 + z, colori); ret=1; } }
      if(inScreen(x0 + x, y0 + y, z0 - z)) if(notColored(x0 + x, y0 + y, z0 - z)) {setColorPixel(x0 + x, y0 + y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 + x, y0 - y, z0 + z)) if(notColored(x0 + x, y0 - y, z0 + z)) {setColorPixel(x0 + x, y0 - y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + x, y0 - y, z0 - z)) if(notColored(x0 + x, y0 - y, z0 - z)) {setColorPixel(x0 + x, y0 - y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - x, y0 + y, z0 + z)) if(notColored(x0 - x, y0 + y, z0 + z)) {setColorPixel(x0 - x, y0 + y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - x, y0 + y, z0 - z)) if(notColored(x0 - x, y0 + y, z0 - z)) {setColorPixel(x0 - x, y0 + y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - x, y0 - y, z0 + z)) if(notColored(x0 - x, y0 - y, z0 + z)) {setColorPixel(x0 - x, y0 - y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - x, y0 - y, z0 - z)) if(notColored(x0 - x, y0 - y, z0 - z)) {setColorPixel(x0 - x, y0 - y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 + y, y0 + x, z0 + x)) if(notColored(x0 + y, y0 + x, z0 + x)) {setColorPixel(x0 + y, y0 + x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + y, y0 + x, z0 - x)) if(notColored(x0 + y, y0 + x, z0 - x)) {setColorPixel(x0 + y, y0 + x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 + y, y0 - x, z0 + x)) if(notColored(x0 + y, y0 - x, z0 + x)) {setColorPixel(x0 + y, y0 - x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + y, y0 - x, z0 - x)) if(notColored(x0 + y, y0 - x, z0 - x)) {setColorPixel(x0 + y, y0 - x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - y, y0 + x, z0 + x)) if(notColored(x0 - y, y0 + x, z0 + x)) {setColorPixel(x0 - y, y0 + x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - y, y0 + x, z0 - x)) if(notColored(x0 - y, y0 + x, z0 - x)) {setColorPixel(x0 - y, y0 + x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - y, y0 - x, z0 + x)) if(notColored(x0 - y, y0 - x, z0 + x)) {setColorPixel(x0 - y, y0 - x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - y, y0 - x, z0 - x)) if(notColored(x0 - y, y0 - x, z0 - x)) {setColorPixel(x0 - y, y0 - x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(del < 0) del += incH;
    else{
      y--;
      if(inScreen(x0 + x, y0 + y, z0 + z)) if(notColored(x0 + x, y0 + y, z0 + z)) {setColorPixel(x0 + x, y0 + y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + x, y0 + y, z0 - z)) if(notColored(x0 + x, y0 + y, z0 - z)) {setColorPixel(x0 + x, y0 + y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 + x, y0 - y, z0 + z)) if(notColored(x0 + x, y0 - y, z0 + z)) {setColorPixel(x0 + x, y0 - y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + x, y0 - y, z0 - z)) if(notColored(x0 + x, y0 - y, z0 - z)) {setColorPixel(x0 + x, y0 - y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - x, y0 + y, z0 + z)) if(notColored(x0 - x, y0 + y, z0 + z)) {setColorPixel(x0 - x, y0 + y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - x, y0 + y, z0 - z)) if(notColored(x0 - x, y0 + y, z0 - z)) {setColorPixel(x0 - x, y0 + y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - x, y0 - y, z0 + z)) if(notColored(x0 - x, y0 - y, z0 + z)) {setColorPixel(x0 - x, y0 - y, z0 + z, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - x, y0 - y, z0 - z)) if(notColored(x0 - x, y0 - y, z0 - z)) {setColorPixel(x0 - x, y0 - y, z0 - z, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 + y, y0 + x, z0 + x)) if(notColored(x0 + y, y0 + x, z0 + x)) {setColorPixel(x0 + y, y0 + x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + y, y0 + x, z0 - x)) if(notColored(x0 + y, y0 + x, z0 - x)) {setColorPixel(x0 + y, y0 + x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 + y, y0 - x, z0 + x)) if(notColored(x0 + y, y0 - x, z0 + x)) {setColorPixel(x0 + y, y0 - x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 + y, y0 - x, z0 - x)) if(notColored(x0 + y, y0 - x, z0 - x)) {setColorPixel(x0 + y, y0 - x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - y, y0 + x, z0 + x)) if(notColored(x0 - y, y0 + x, z0 + x)) {setColorPixel(x0 - y, y0 + x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - y, y0 + x, z0 - x)) if(notColored(x0 - y, y0 + x, z0 - x)) {setColorPixel(x0 - y, y0 + x, z0 - x, colori); ret=1;  printf("dac\n");}

      if(inScreen(x0 - y, y0 - x, z0 + x)) if(notColored(x0 - y, y0 - x, z0 + x)) {setColorPixel(x0 - y, y0 - x, z0 + x, colori); ret=1;  printf("dac\n");}
      if(inScreen(x0 - y, y0 - x, z0 - x)) if(notColored(x0 - y, y0 - x, z0 - x)) {setColorPixel(x0 - y, y0 - x, z0 - x, colori); ret=1;  printf("dac\n");}
      incO += 4;
      del += incO;
    }
  }//end for
  return ret;
}


int diagGrow(){
  int i, fini = 1;
  Color color;

  for(i=0; i<NBCOLOR; i++){
    //printf("%d iiiii \n",i);
  //  if(!diag[i].alive) {continue;}

    color.r = diag[i].x/255;
    color.g = diag[i].y/255;
    color.b = diag[i].z/255;
    //diag[i].color.r = color.r;
    //diag[i].color.g = color.g;
    //diag[i].color.b = color.b;


    if(circleNOM(diag[i].x,diag[i].y,diag[i].z, color.r,color.g,color.b)){
      //printf("fini = 0 \n");
      fini=0;
    }
    else{
      diag[i].alive=0;
      //printf("finish voronoi number%d \n",i);
    }
  }
  radius++;
  printf("finiii ??? %d\n", !fini);
  return !fini;
}

void maclutInit(){
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
          ma_clut[i][j][k].r=444.0f;
          ma_clut[i][j][k].g=444.0f;
          ma_clut[i][j][k].b=444.0f;
        }
      }
    }
  }

void diagInit(){
  maclutInit();
  int i, x,y,z;
  radius = 1;
  for(i=0; i<NBCOLOR; i++){ //first : select random coordinate(x,y,z)

    //x = rand_a_b(0,SIZECOLOR);
    //y = rand_a_b(0,SIZECOLOR);
    //z = rand_a_b(0,SIZECOLOR);
    x=1;
    y=1;
    z=1;

    diag[i].x = x;
    diag[i].y = y;
    diag[i].z = z;
    diag[i].alive = 1;

    ma_clut[x][y][z] = getColor(x,y,z); //set the point color to a random position
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
    }
  }
}

void voronoi(){
/*  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0;k<SIZECOLOR; k++){
      }
    }
  }*/
//  diagInit();
//  diagGrow();

/*
  diagInit();
  if(!diagGrow()) {//si cest fini , refaire
    diagInit();
  }
  */

/*
  diagInit();


  do{
    diagGrow();
  }
  while(!isAllColored());
*/

//------------
/*  diagInit();
  while(!isAllColored()){
    if(!diagGrow()){
      affiche();

    }
  }*/
//-------------
  diagInit();
  while(diagGrow() != 0){
    printf("pas eeeeeeeeeeeeecnooreuuu \n");
  }

  affiche();

  return;
}
