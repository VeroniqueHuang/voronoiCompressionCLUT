  #include <unistd.h>
#include <math.h>
#include "ima.h"


#include <string.h>

#include <time.h>
#include "clut.h"

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
  if(radius==1){
    if(inScreen(x0, y0, z0)) {if(notColored(x0,y0,z0)) {setColorPixel(x0,y0,z0, r,g,b,nb); ret=1;}}//place un pixel au centre/depart
  }
  if(radius==2){
    for(int i=((x0-1 <0)? 0 : x0-1);  i<=((x0+1 >SIZECOLOR)? SIZECOLOR : x0+1);  i++){
      for(int j=((y0-1 <0)? 0 : x0-1);  j<=((y0+1 >SIZECOLOR)? SIZECOLOR : y0+1);  j++){
        for(int k=((z0-1 <0)? 0 : x0-1);  k<=((z0+1 >SIZECOLOR)? SIZECOLOR : z0+1);  k++){
          if(inScreen(i, j, k)) {if(notColored(i, j, k)) {setColorPixel(i, j, k, r,g,b,nb); ret=1;}} //creer un cube de taille 3x3x3
        }
      }
    }
  }
  else{//algorithme pour creer une sphere dans tous ces etages


    for(int i=((x0-radius-1 <0)? 0 : x0-radius);  i<((x0+radius >=0)? SIZECOLOR : x0+radius);  i++){
      for(int j=((y0-radius-1 <0)? 0 : y0-radius);  j<((y0+radius >=0)? SIZECOLOR : y0+radius);  j++){
        for(int k=((z0-radius-1 <0)? 0 : z0-radius);  k<((z0+radius >=0)? SIZECOLOR : z0+radius);  k++){
          //printf("okk \n");
            //------------------------------------------------------------------------
            if((ma_clut[i][j][k].diagnb == nb) && ((ma_clut[i][j][k].cub == radius) ||(ma_clut[i][j][k].cub == radius-1)) ){
            //  x0=x0+radius;
            //  y0=y0+radius;
            //  z0=z0+radius;

              if(inScreen(i+1, j, k)) {if(notColored(i+1, j, k)) {setColorPixel(i+1, j, k, r,g,b,nb); ret=1;}}
              if(inScreen(i-1, j, k)) {if(notColored(i-1, j, k)) {setColorPixel(i-1, j, k, r,g,b,nb); ret=1;}}

              if(inScreen(i, j+1, k)) {if(notColored(i, j+1, k)) {setColorPixel(i, j+1, k, r,g,b,nb); ret=1;}}
              if(inScreen(i, j-1, k)) {if(notColored(i, j-1, k)) {setColorPixel(i, j-1, k, r,g,b,nb); ret=1;}}

              if(inScreen(i, j, k+1)) {if(notColored(i, j, k+1)) {setColorPixel(i, j, k+1, r,g,b,nb); ret=1;}}
              if(inScreen(i, j, k-1)) {if(notColored(i, j, k-1)) {setColorPixel(i, j, k-1, r,g,b,nb); ret=1;}}
            }//end outer if

          }
        }
      }//end for


  }

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

/*
int doublon(int x, int y, int z){
  return 0;
}*/

void diagInit(){
  maclutInit();
  int i, x,y,z;
  radius = 1;
  cube = 1;
  for(i=0; i<NBCOLOR; i++){ //first : select random coordinate(x,y,z)

    do{
      x = rand_a_b(0,SIZECOLOR-1);
      y = rand_a_b(0,SIZECOLOR-1);
      z = rand_a_b(0,SIZECOLOR-1);

    }while(doublon(x,y,z));
  //  x =4;
  //  y =4;
  //  z =4;

//  printf("%d %d %d \n", x,y,z);
    doublons[i].x=x;
    doublons[i].y=y;
    doublons[i].z=z;

    diag[i].x = x;
    diag[i].y = y;
    diag[i].z = z;
    diag[i].alive = 1;

    diag[i].color = getColor(x,y,z); //set the point color to a random position

    if(i==0){couleura.r = diag[i].color.r; couleura.g = diag[i].color.g; couleura.b = diag[i].color.b;}
    if(i==1){couleurb.r = diag[i].color.r; couleurb.g = diag[i].color.g; couleurb.b = diag[i].color.b;}

    ma_clut[x][y][z] = diag[i].color;
    ma_clut[x][y][z].cub = cube;
    ma_clut[x][y][z].diagnb = i;

  }
}

void afficheCLUT(){
  printf("-----------------\n\n");
  for(int i=0; i<SIZECOLOR; i++){
    for(int j=0; j<SIZECOLOR; j++){
      for(int k=0; k<SIZECOLOR; k++){
        printf("-------%f %f %f\n",ma_clut[i][j][k].r,ma_clut[i][j][k].g,ma_clut[i][j][k].b);

      /*
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
    printf("________\n");*/
      }
    }
  }//---keep
}


void voronoi(){
  srand(time(NULL));

  diagInit();
  while( diagGrow() != 0 ){
    //afficheCLUT();
    //if(radius==5){
      //break;
    //}
  }
  printf("Clut END\n");
  //afficheCLUT();



  return;
}
//BINARY
char * binaire(int nb){
  int a[300],i,j;
  for(i=1; nb>0; i++){
    a[i]=nb%2;
    nb=nb/2;
  }
  char *k = malloc(i*sizeof(char));

  for(i=i-1,j=0;i>=0;i--,j++){
    printf("%d",a[i]);
    k[j] =  a[i] + '0';
  }

return k;

}
//HEXA
int hexaf(int decimal){
long quotient, remainder;
int i, j = 0;
char hexadecimal[100];

quotient = decimal;

while (quotient != 0)
{
remainder = quotient % 16;          //step 1
if (remainder < 10)
hexadecimal[j++] = 48 + remainder;   //step 2
else
hexadecimal[j++] = 55 + remainder;   //step 3
quotient = quotient / 16;            //step 4
}
printf("Equivalent hexadecimal value of decimal number %ld:",decimal);
//step 6
for (i = j; i >= 0; i--)
printf("%c \n", hexadecimal[i]);
return 0;
}
//---------IMAGE---------------

Color getPixelColor(GLint x, GLint y) {
    Color color;
    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
    return color;
}

float * gg(GLint x, GLint y) {
  GLubyte pick_col[3];
  glFlush();
  float *f = malloc(3);
  glReadPixels(y , x , 1 , 1 , GL_RGB , GL_UNSIGNED_BYTE , &pick_col[0]);
  f[0] = pick_col[0];
  f[1]= pick_col[1];
  f[2] = pick_col[2];
  //printf("%f %f %f color\n",f[0],f[1],f[2] );
  //printf("%f %f %f dadaaaa\n",f[0],f[1],f[2]);
  return f;
}

void compression(char *filename, Image *img){
   int i, j,l;
   float * tab;
   FILE *fp = fopen(filename, "wb"); /* b - binary mode */
   fprintf(fp, "%lu %lu %lu\n",img->sizeX,img->sizeY,SIZECOLOR);
   static unsigned char color[3];
   int nombre;
   int num=-1;
   int n=0;
   int keke=0;
   //CLUT
   for(l=0; l<NBCOLOR; l++){
     fprintf(fp, "%d,%f,%f,%f \n",l, diag[l].color.r, diag[l].color.g, diag[l].color.b);
  }
   fprintf(fp,"/");

   num=-1; n=0;

   //IMAGE INDEX
     for (i = 0; i <HEIGHT; i++){//x
       n=0;
       num=-1;
       for (j = 0; j<WIDTH; j++){//y

        tab=gg(i,j);

        color[0] = *(tab + 0)*255;  /* red */
        color[1] = *(tab + 1)*255;  /* green */
        color[2] = *(tab + 2)*255;  /* blue */

        nombre = ma_clut[(int)(*(tab + 0)*SIZECOLOR/255)][(int)(*(tab + 1)*SIZECOLOR/255)][(int)(*(tab + 2)*SIZECOLOR/255)].diagnb;

        //printf("%d && %d\n",nombre , num);
        if(num == nombre){
          hashmap[n].stock=hashmap[n].stock+1;
          //printf("%d %d \n",num,nombre);
        }

        else{
          n++;
          num = nombre;
          //printf("%d ",nombre);
          hashmap[n].nombre = num;
          hashmap[n].stock = 1;
        }
     }

  for (int t = 1; t <= n; t++){
    if(hashmap[t].stock == 1){ fprintf(fp, "%d ",hashmap[t].nombre);}
    else{ fprintf(fp, "%d*%d ", hashmap[t].stock, hashmap[t].nombre);}
  //  printf(" %d %d, ",hashmap[t].nombre,i);

  }
}//close outer for
   free(tab);
   (void) fclose(fp);
}

void decompression(char *filename){

/*
   char buff[255];
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff );
   fscanf(fp, "%s", buff);
   printf("1 : %s\n", buff ); */


}

char* append(char* s, char c) {
        int len = strlen(s);
        s[len] = c;
        s[len+1] = '\0';
        return s;
}

int loadMyImage(char *filename, Image *img){
  //img = (Image *) malloc(sizeof(Image));
  int size;
  FILE *fp;
  fp = fopen(filename, "rw");
GLubyte * im;
im = img->data;
  //char *str = malloc(10*sizeof(char*));

  //str = "";
   //strcpy(str,"");
  char ch;
  int index = 0;

  //read image size information
  int tailleClut;
  if (fscanf(fp, "%lu %lu %d", &img->sizeX, &img->sizeY, &tailleClut) != 3) {
       fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
       exit(1);
  }

  size = img->sizeX * img->sizeY * 3;
  printf("Size image %lu %lu => %d %d\n", img->sizeX, img->sizeY, size, tailleClut);
  //img->data = (GLubyte *) malloc ((size_t) size * sizeof (GLubyte));


  myGlubyte ColorArray[NBCOLOR];
  myGlubyte couleur;
  int number;
  int nombre,stock;

  int etoile;
  int s=0;
  //int s=img->sizeX * img->sizeY;

  while( (ch = fgetc(fp)) != '/'){
    fscanf(fp, "%d,%f,%f,%f", &number, &couleur.r, &couleur.g, &couleur.b);
    //printf("%d = %f %f %f -%d\n ",number, couleur.r, couleur.g, couleur.b,ff);
    ColorArray[number] = couleur;
  }
char str[100000];
int bb=0;
str[0] = '\0';
  while ((ch = fgetc(fp)) != EOF){
  //  printf("%c ",ch);
      //printf("%d\n",s );
        if(ch == 42){//star '*'
          stock = atoi(str);
          //printf("%d* ",stock);
          bb+=stock;
          //str[0] = '\0';
           memset( str, 0, sizeof (str) );
          index = 0;
          etoile=1;
        }
        else if(ch == 32){//space ' '
          if(etoile==1){
            nombre = atoi(str);
            //printf("%d \n",nombre);
            while(stock!=1){
              im[s*3]=ColorArray[nombre].r;
              im[s*3+1]=ColorArray[nombre].g;
              im[s*3+2]=ColorArray[nombre].b;
              s++ ;stock--;
            }
            nombre=0;stock=0;
            etoile=0;
           }
          else{
            bb++;
            nombre = atoi(str);
            //printf("-%d \n",nombre);
            im[s*3]=ColorArray[nombre].r;
            im[s*3+1]=ColorArray[nombre].g;
            im[s*3+2]=ColorArray[nombre].b;
            s++ ;
          }
            //str[0] = '\0';
            memset( str, 0, sizeof (str) );
            index = 0;
        }// ' '
        else{
          str[index]=ch;
           index++;
        }
  }//end while
printf("%d rbeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeb\n",bb);
  str[0] = '\0';//free space
/*
    while ((ch = fgetc(fp)) != EOF){
      //printf("%d ",ch);
      if(ch == '*'){
        //printf("%c",ch);
          stock = atoi(str);
          str[0] = '\0';index = 0;
        //  while(ch != ' ' ){str[index] = ch;index++;}
          nombre = atoi(str);
          while(stock>0){
            //im[s*3]=ColorArray[nombre].r;
            //im[s*3+1]=ColorArray[nombre].g;
            //im[s*3+2]=ColorArray[nombre].b;
            s++;stock--;
          }
          //str[0] = '\0';
            str[0] = '\0';index = 0;
      }
      else if (ch == ' '){
        //im[s*3]=ColorArray[nombre].r;
        //im[s*3+1]=ColorArray[nombre].g;
        //im[s*3+2]=ColorArray[nombre].b;
        s++;
        str[0] = '\0';index = 0;
      }
      else{//if number
        str[index] = ch;index++;
      }
    }
*/
    //-------------------------------------
  /*
    while ((ch = fgetc(fp)) != EOF){

        printf("%c",ch);
    }
  */
    //-------------------------------------


/*
  while ((ch = fgetc(fp)) != EOF){
    if(ch == '['){
      cl=1;
    }
    else if (ch == ']'){
      cl=0;
    }
    if(cl==1){//in clut file
      if(ch=='*'){
        stock = atoi(str); //convert in integer
        printf("stock %d ",stock );
        b=1;
         str[0] = '\0';
         continue;
       }
       else if(ch==' '){
         if(b==1){
           nombre = atoi(str); //convert number in integer
           printf("nombre %d \n",nombre );
           b=0;
          while(stock>0){
            nouvelle_clut[p][q][r]=nombre;
            if(r==SIZECOLOR){r=0;q++;}
            if(q==SIZECOLOR){q=0;p++;}
             stock--;
           }
         }
         else{//si nombre sans '*'
           nouvelle_clut[p][q][r]=nombre;
           if(r==SIZECOLOR){r=0;q++;}
           if(q==SIZECOLOR){q=0;p++;}
         }
         printf("%s ",str);
         str[0] = '\0';
         continue;
       }
    }



   else if(ch=='*'){
     stock = atoi(str); //convert in integer
     printf("stock %d ",stock );
     b=1;
      //printf("%c", ch);
      str[0] = '\0';
      continue;
    }
    else if(ch==' '){
      if(b==1){
        nombre = atoi(str); //convert number in integer
        printf("nombre %d \n",nombre );
        b=0;
       while(stock>0){
         img->data = val;
          //img->data[dat] = nombre*255/tailleClut;
          dat++;
          stock--;
        }
      }
      else{


      }
      //printf("%s ",str);
      str[0] = '\0';
      continue;
    }
    else{//if number
      strncat(str, &ch, 1);
    }
  }//end while */

/*
GLubyte  val=200;
myGlubyte color;
  color.r=255;
  color.g=0;
  color.b=255;
  for (int j = 0; j < 20000; j ++) {
    im[j*3]=color.r;
    im[j*3+1]=color.g;
    im[j*3+2]=color.b;
  }
*/
/*  if (fread(img->data, (size_t) 1, (size_t) size, fp) == 0) {
       fprintf(stderr, "Error loading image '%s'\n", filename);
     }*/

  fclose(fp);
  return EXIT_SUCCESS;
}
