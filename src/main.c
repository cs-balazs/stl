#include <stdio.h>
#include "stl.h"

int main()
{
  STL stl = stl_read("./assets/suzanne.stl");

  printf("%s\n", stl.header);
  printf("stl.indices_count = %d\n", stl.indices_count);
  printf("stl.vertices_count = %d\n", stl.vertices_count);

  printf("Normals:\n[\n");
  for (size_t i = 0; i < stl.indices_count / 3; i++)
  {
    uint16_t normals_index = stl.normal_indices[i] * 3;
    printf("  [ %f, %f, %f ]\n", stl.normals[normals_index], stl.normals[normals_index + 1], stl.normals[normals_index + 2]);
  }
  printf("]\n");

  stl_write(stl, "./assets/suzanne2.stl");

  stl_free(stl);
  return 0;
}