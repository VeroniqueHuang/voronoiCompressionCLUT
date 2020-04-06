/* il s agit de prendre une image et de la modifier */

#include "ima.h"
#include <limits.h>



void gris_uniforme(Image * i) {
  int j,ii, size;
  GLubyte * im, val;
  val = 128;
  size = 3 * i->sizeY * i->sizeX;
  im = i->data;
  for (j = 0; j < size; j ++) {
	*im++ = val;
  printf("%d %d %d %d\n",im[0],im[1],im[3],im[4]);
  }
}
