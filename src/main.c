#include <stdio.h>
#include "stl.h"

int main()
{
  STL stl = stl_read("./assets/suzanne.stl");

  printf("%s\n", stl.header);
  printf("stl.indices_count = %d\n", stl.indices_count);
  printf("stl.vertices_count = %d\n", stl.vertices_count);

  stl_free(stl);
  return 0;
}