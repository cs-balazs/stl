#include <stdio.h>
#include "stl.h"

int main()
{
  STLReadResult stl = stl_read("./assets/cube.stl");

  printf("stl.indices_count = %d\n", stl.indices_count);
  printf("stl.vertices_count = %d\n", stl.vertices_count);

  return 0;
}