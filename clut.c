#include <unistd.h>
#include <math.h>
#include "ima.h"
#include <string.h>
#include <time.h>
#include "clut.h"

//---------------Creating The Color Lookup Table---------------
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


int notColored(int x, int y, int z){//if pixel(x,y,z) is not colored
  if(ma_clut[x][y][z].r ==-10.0f && ma_clut[x][y][z].g==-10.0f && ma_clut[x][y][z].b==-10.0f)
    return 1;
  return 0;
}

int inScreen(int x, int y, int z){
  if((x>=0 && x<SIZECOLOR) && (y>=0 && y<SIZECOLOR) && (z>=0 && z<SIZECOLOR))
    return 1;
  return 0;
}

void setColorPixel(int x, int y, int z, GLfloat r, GLfloat g, GLfloat b, int nb){//set color(r,g,b) to pixel(x,y,z)
  ma_clut[x][y][z].r = (GLfloat)r;
  ma_clut[x][y][z].g = (GLfloat)g;
  ma_clut[x][y][z].b = (GLfloat)b;
  ma_clut[x][y][z].cub = radius+1;
  ma_clut[x][y][z].indexColor = nb;
}

int growSphere(int x0, int y0, int z0, GLfloat r, GLfloat g, GLfloat b, int nb){//function of voronoi in 3D
  int ret=0;
  if(radius==1){
    if(inScreen(x0, y0, z0)) {if(notColored(x0,y0,z0)) {setColorPixel(x0,y0,z0, r,g,b,nb); ret=1;}}//put pixel in origin
  }
  if(radius==2){
    for(int i=((x0-1 <0)? 0 : x0-1);  i<=((x0+1 >SIZECOLOR)? SIZECOLOR : x0+1);  i++){
      for(int j=((y0-1 <0)? 0 : x0-1);  j<=((y0+1 >SIZECOLOR)? SIZECOLOR : y0+1);  j++){
        for(int k=((z0-1 <0)? 0 : x0-1);  k<=((z0+1 >SIZECOLOR)? SIZECOLOR : z0+1);  k++){
          if(inScreen(i, j, k)) {if(notColored(i, j, k)) {setColorPixel(i, j, k, r,g,b,nb); ret=1;}} //create a cube of 3x3x3
        }
      }
    }
  }
  else{//create sphere that grow from the origin
    for(int i=((x0-radius-1 <0)? 0 : x0-radius);  i<((x0+radius >=0)? SIZECOLOR : x0+radius);  i++){
      for(int j=((y0-radius-1 <0)? 0 : y0-radius);  j<((y0+radius >=0)? SIZECOLOR : y0+radius);  j++){
        for(int k=((z0-radius-1 <0)? 0 : z0-radius);  k<((z0+radius >=0)? SIZECOLOR : z0+radius);  k++){
            if((ma_clut[i][j][k].indexColor == nb) && ((ma_clut[i][j][k].cub == radius) ||(ma_clut[i][j][k].cub == radius-1)) ){

              if(inScreen(i+1, j, k)) {if(notColored(i+1, j, k)) {setColorPixel(i+1, j, k, r,g,b,nb); ret=1;}}
              if(inScreen(i-1, j, k)) {if(notColored(i-1, j, k)) {setColorPixel(i-1, j, k, r,g,b,nb); ret=1;}}

              if(inScreen(i, j+1, k)) {if(notColored(i, j+1, k)) {setColorPixel(i, j+1, k, r,g,b,nb); ret=1;}}
              if(inScreen(i, j-1, k)) {if(notColored(i, j-1, k)) {setColorPixel(i, j-1, k, r,g,b,nb); ret=1;}}

              if(inScreen(i, j, k+1)) {if(notColored(i, j, k+1)) {setColorPixel(i, j, k+1, r,g,b,nb); ret=1;}}
              if(inScreen(i, j, k-1)) {if(notColored(i, j, k-1)) {setColorPixel(i, j, k-1, r,g,b,nb); ret=1;}}
            }

          }
        }
      }//end outer for
    }//end else
  return ret;
}

int ClutIndexColorGrow(){
  int i, fini = 1;
  for(i=0; i<NBCOLOR; i++){
    if(growSphere(ClutIndexColor[i].x,ClutIndexColor[i].y,ClutIndexColor[i].z, ClutIndexColor[i].color.r, ClutIndexColor[i].color.g, ClutIndexColor[i].color.b, i)){
      fini=0;
    }
    else{
      ClutIndexColor[i].alive=0;
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

int repeatNumber(int x, int y,int z){
  if(visited[x][y][z])
    return 1;
  return 0;
}
void ClutIndexColorInit(){
  maclutInit();
  int i, x,y,z;
  radius = 1;
  cube = 1;
  for(i=0; i<NBCOLOR; i++){
    do{ //select random coordinate(x,y,z)
      x = rand_a_b(0,SIZECOLOR-1);
      y = rand_a_b(0,SIZECOLOR-1);
      z = rand_a_b(0,SIZECOLOR-1);
    }while(repeatNumber(x,y,z)); //avoid repeating number
    visited[x][y][z]=1;

    ClutIndexColor[i].x = x;
    ClutIndexColor[i].y = y;
    ClutIndexColor[i].z = z;
    ClutIndexColor[i].alive = 1;
    ClutIndexColor[i].color = getColor(x,y,z);//set the point color to a random position

    ma_clut[x][y][z] = ClutIndexColor[i].color;
    ma_clut[x][y][z].cub = cube;
    ma_clut[x][y][z].indexColor = i;
  }
}

void voronoi(){
  srand(time(NULL));
  ClutIndexColorInit();
  while( ClutIndexColorGrow() != 0 ){
  }
  printf("Clut END\n");
  return;
}

//---------------Compression---------------
  float * readPixels(GLint x, GLint y) {//collect the color of pixel(x,y)
    GLubyte pick_col[3];
    glFlush();
    float *colorf = malloc(3);
    glReadPixels(y , x , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , &pick_col[0]);
    colorf[0] = pick_col[0];
    colorf[1]= pick_col[1];
    colorf[2] = pick_col[2];
    return colorf;
  }

void compressionImg(char *filename, Image *img){
   int i,j,m;
   float * tab;
   FILE *fp = fopen(filename, "wb");
   fprintf(fp, "%lu %lu %lu\n",img->sizeX,img->sizeY,SIZECOLOR);
   int nombre;
   int temp=-1;
   int n=0;
    //-------Write data of the Clut in file-------
   for(m=0; m<NBCOLOR; m++){
     fprintf(fp, "%d,%f,%f,%f \n",m, ClutIndexColor[m].color.r, ClutIndexColor[m].color.g, ClutIndexColor[m].color.b);
  }
   fprintf(fp,"/");

     for (i = 0; i <HEIGHT; i++){
       n=0;temp=-1;
      //-------Write data of each pixel (convert in index of color)-------
       for (j = 0; j<WIDTH; j++){
        tab=readPixels(i,j);
        nombre = ma_clut[(int)(*(tab + 0)*SIZECOLOR/255)][(int)(*(tab + 1)*SIZECOLOR/255)][(int)(*(tab + 2)*SIZECOLOR/255)].indexColor;
        if(temp == nombre){
          recurrence[n].stock=recurrence[n].stock+1;
        }
        else{
          n++;
          temp = nombre;
          recurrence[n].nombre = temp;
          recurrence[n].stock = 1;
        }
     }

  for (int t = 1; t <= n; t++){
    if(recurrence[t].stock == 1){ fprintf(fp, "%d ",recurrence[t].nombre);}
    else{ fprintf(fp, "%d*%d ", recurrence[t].stock, recurrence[t].nombre);}
  }
}//close outer for
   free(tab);
   (void) fclose(fp);
}

//---------------Decompression---------------
int loadMyImage(char *filename, Image *img){
  int size;
  FILE *fp;
  fp = fopen(filename, "rw");
  GLubyte * im;
  im = img->data;
  char ch;
  int index = 0;
  char str[WIDTH +1];

  //read image size information
  int tailleClut;
  if (fscanf(fp, "%lu %lu %d", &img->sizeX, &img->sizeY, &tailleClut) != 3) {
       fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
       exit(1);
  }
  size = img->sizeX * img->sizeY * 3;
  printf("Size image %d %d => %d %d\n", img->sizeX, img->sizeY, size, tailleClut);

  myGlubyte ColorArray[NBCOLOR];
  myGlubyte couleur;
  int number,nombre, stock, etoile, s=0;

  while( (ch = fgetc(fp)) != '/'){
    fscanf(fp, "%d,%f,%f,%f", &number, &couleur.r, &couleur.g, &couleur.b);
    ColorArray[number] = couleur;
  }
  memset( str, 0, sizeof (str) );
  while ((ch = fgetc(fp)) != EOF){
        if(ch == 42){//if 'ch' is a star '*'
          stock = atoi(str);
          memset( str, 0, sizeof (str) );//empty the string
          index = 0;
          etoile=1;
        }
        else if(ch == 32){//if 'ch' is a space ' '
          if(etoile==1){
            nombre = atoi(str);
            while(stock!=0){
              im[s*3]=ColorArray[nombre].r;
              im[s*3+1]=ColorArray[nombre].g;
              im[s*3+2]=ColorArray[nombre].b;
              s++ ;stock--;
            }
            nombre=0;stock=0;
            etoile=0;
           }
          else{
            nombre = atoi(str);
            im[s*3]=ColorArray[nombre].r;
            im[s*3+1]=ColorArray[nombre].g;
            im[s*3+2]=ColorArray[nombre].b;
            s++ ;
          }
          memset( str, 0, sizeof (str) );
          index = 0;
        }
        else{
          str[index]=ch;
          index++;
        }
  }//end while
  str[0] = '\0';//free space
  fclose(fp);
  return EXIT_SUCCESS;
}
