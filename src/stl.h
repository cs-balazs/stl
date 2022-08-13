#ifndef STL_HEADER
#define STL_HEADER

#include <stdlib.h>
#include <stdint.h>

#define STL_HEADER_LENGTH 80
#define STL_FACET_LENGTH 50

typedef struct STL
{
  char *header;
  float *vertices;
  uint16_t *indices;
  size_t vertices_count;
  size_t indices_count;
} STL;

void stl_free(STL stl);

STL stl_read(const char *file_path);

void stl_write(const char *file_path);

#endif