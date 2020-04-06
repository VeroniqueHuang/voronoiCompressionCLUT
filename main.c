#include <unistd.h>
#include <math.h>
#include "ima.h"

#include <GL/glut.h>
#include <GL/glu.h>

Image *image;

#define ESCAPE 27

void Keyboard(unsigned char key, int x, int y)  {
  switch(key){
  case ESCAPE :
    exit(0);
    break;
  default:
    fprintf(stderr, "Unused key\n");
  }
}

void Mouse(int button, int state, int x, int y) {

  switch(button){
  case GLUT_LEFT_BUTTON:
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  }
  glutPostRedisplay();
}

int Init(char *s) {

  image = (Image *) malloc(sizeof(Image));
  if (image == NULL) {
    fprintf(stderr, "Out of memory\n");
    return(-1);
  }
  if (ImageLoad_PPM(s, image)==-1)
	return(-1);
  printf("tailles %d %d\n",(int) image->sizeX, (int) image->sizeY);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);

  return (0);
}
int ReInit() {

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glutReshapeWindow(image->sizeX, image->sizeY);

  return (0);
}

void Display(void) {

  GLint w, h;

  glClear(GL_COLOR_BUFFER_BIT);

  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  glDrawPixels(image->sizeX, image->sizeY, GL_RGB, GL_UNSIGNED_BYTE,
	       image->data);

  glFlush();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, (GLdouble) w, 0.0, (GLdouble)h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void menuFunc(int item) {
  char s[256];
  char ss[30]="veroniqueimg.txt";

  switch(item){
  case 0:
    free(image);
    exit(0);
    break;
  case 1:
  case 2:
  case 3:
  voronoi();
  //afficheCLUT();
  printf("COMPRESSION DEBUT\n");
  compression(ss, image);
  printf("COMPRESSION END\n");
  printf("----DECOMPRESSION DEBUT\n");
  //decompression(ss);
  loadMyImage(ss);
  printf("----DECOMPRESSION END\n");
  ReInit();

    //display();
    break;
  case 4:
  printf("Tout gris\n");
	gris_uniforme(image);
	Display();
    break;
  case 5:
    printf("Entrer le nom pour l'image dans cette taille\n");
    scanf("%s", &s[0]);
    imagesave_PPM(s, image);
    break;
  case 6:
    printf("Taille de l image : %ld %ld\n", (int) image->sizeX, (int) image->sizeY);
    break;
  default:
    break;
  }
}

int main(int argc, char **argv) {

  if (argc<2) {
    fprintf(stderr, "Usage : palette nom_de_fichier\n");
    exit(0);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
  glutInitWindowSize(640,480);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("VPUP8");

  Init(argv[1]);

  glutCreateMenu(menuFunc);
  glutAddMenuEntry("Quit", 0);
  glutAddMenuEntry("gris", 1);
  glutAddMenuEntry("Gris", 2);
  glutAddMenuEntry("GRIS", 3);
  glutAddMenuEntry("grey", 4);
  glutAddMenuEntry("Sauver", 5);
  glutAddMenuEntry("Informations", 6);
  glutAttachMenu(GLUT_LEFT_BUTTON);

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Keyboard);

  glutMouseFunc(Mouse);

  glutMainLoop();

  return 1;
}
