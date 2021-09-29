#include "utils.h"

// Converts 1D coords to 2D
V2 to2D(int index, int width){
      V2 vect;
      vect.y = index / width;
      vect.x = index % width;

      return vect;
}

// Converts 2D coords to 1D
int to1D(V2* vect, int width){
      return vect->y * width + vect->x;
}