/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>


GLubyte rand_a_ (int a, int b){
  return rand()%(b-a)+a;
}

void gris_uniforme(Image * i) {
  int j,ii, size;
  GLubyte * im, val;
  val = 128;
  size = 3 * i->sizeY * i->sizeX;
  im = i->data;
  for (j = 0; j < size; j ++) {
    GLubyte  x = rand_a_b(0,200);
	  *im++ = x;
  }
}
