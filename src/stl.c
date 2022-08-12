#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stl.h"

typedef struct ReadFileResult
{
  char *bytes;
  size_t length;
} ReadFileResult;

ReadFileResult read_file(const char *filename)
{
  FILE *fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0, SEEK_END);
  long length = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  char *buffer = malloc(length + 1);

  fread(buffer, 1, length, fp);
  buffer[length] = 0,

  fclose(fp);
  return (ReadFileResult){buffer, length};
};

STLReadResult stl_read(const char *file_path)
{

  ReadFileResult stl = read_file(file_path);
  char header[STL_HEADER_LENGTH];
  memcpy(header, stl.bytes, STL_HEADER_LENGTH);

  uint32_t facets;
  memcpy(&facets, stl.bytes + 80, 4);

  size_t num_of_vertices = facets * 3 * 3;
  float *vertices = malloc(num_of_vertices * sizeof(float) * 10);
  uint16_t *indices = malloc((num_of_vertices / 3) * sizeof(uint16_t) * 10);
  size_t vertex_index = 0;
  size_t indices_index = 0;

  for (size_t i = 84; i < stl.length; i += STL_FACET_LENGTH)
  {
    for (size_t vertex_offset = i + 12; vertex_offset <= i + 36;
         vertex_offset += 12)
    {
      float new_vertex[3];
      memcpy(new_vertex, stl.bytes + vertex_offset, 12);

      uint16_t duplicate_index = 0;
      for (size_t prev_vertex_index = 0; prev_vertex_index < vertex_index;
           prev_vertex_index += 3)
      {
        if (vertices[prev_vertex_index] == new_vertex[0] &&
            vertices[prev_vertex_index + 1] == new_vertex[1] &&
            vertices[prev_vertex_index + 2] == new_vertex[2])
        {
          duplicate_index = prev_vertex_index;
          break;
        }
      }

      if (duplicate_index > 0)
      {
        indices[indices_index] = duplicate_index / 3u;
      }
      else
      {
        vertices[vertex_index] = new_vertex[0];
        vertices[vertex_index + 1] = new_vertex[1];
        vertices[vertex_index + 2] = new_vertex[2];
        indices[indices_index] = vertex_index / 3u;
        vertex_index += 3;
      }
      indices_index++;
    }
  }

  num_of_vertices = vertex_index / 3;

  vertices = realloc(vertices, vertex_index * sizeof(float));
  indices = realloc(indices, indices_index * sizeof(uint16_t));

  free(stl.bytes);

  return (STLReadResult){vertices, indices, num_of_vertices,
                         indices_index};
}
