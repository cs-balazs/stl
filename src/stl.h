#ifndef STL_HEADER
#define STL_HEADER

#include <stdlib.h>
#include <stdint.h>

#define STL_HEADER_SIZE 80
#define STL_FACET_SIZE 50
#define STL_VERTEX_SIZE 12

// Number of normal vectors = indices_count / 3
typedef struct STL {
  char *header;
  float *vertices;
  uint16_t *indices;
  size_t vertices_count;
  size_t indices_count;
} STL;

STL stl_read(const char *file_path);

void stl_write(STL stl, const char *file_path);

void stl_free(STL stl);

#endif