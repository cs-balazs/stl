#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stl.h"

typedef struct ReadFileResult {
  char *bytes;
  size_t length;
} ReadFileResult;

ReadFileResult read_file(const char *file_path);
void write_file(const char *file_path, char *buffer, size_t buffer_size);

STL stl_read(const char *file_path)
{
  ReadFileResult stl = read_file(file_path);
  char *header = malloc(STL_HEADER_SIZE + 1);
  memcpy(header, stl.bytes, STL_HEADER_SIZE);
  header[STL_HEADER_SIZE] = '\0';

  uint32_t facets;
  memcpy(&facets, stl.bytes + STL_HEADER_SIZE, 4);

  float *vertices = malloc(facets * 6 * STL_VERTEX_SIZE);
  uint16_t *indices = malloc(facets * 3 * sizeof(uint16_t));
  size_t vertex_index = 0ul;
  size_t indices_index = 0ul;

  for (size_t i = 84; i < stl.length; i += STL_FACET_SIZE) {
    float new_normal[3];
    memcpy(new_normal, stl.bytes + i, STL_VERTEX_SIZE);

    for (size_t vertex_offset = i + STL_VERTEX_SIZE;
         vertex_offset <= i + 3 * STL_VERTEX_SIZE;
         vertex_offset += STL_VERTEX_SIZE) {
      float new_vertex[3];
      memcpy(new_vertex, stl.bytes + vertex_offset, STL_VERTEX_SIZE);

      uint16_t duplicate_index = 0;
      for (size_t prev_vertex_index = 0; prev_vertex_index < vertex_index;
           prev_vertex_index += 6) {
        if (vertices[prev_vertex_index] == new_vertex[0] &&
            vertices[prev_vertex_index + 1] == new_vertex[1] &&
            vertices[prev_vertex_index + 2] == new_vertex[2] &&
            vertices[prev_vertex_index + 3] == new_vertex[3] &&
            vertices[prev_vertex_index + 4] == new_vertex[4] &&
            vertices[prev_vertex_index + 5] == new_vertex[5]) {
          duplicate_index = prev_vertex_index;
          break;
        }
      }

      if (duplicate_index > 0) {
        indices[indices_index] = duplicate_index / 6u;
      } else {
        vertices[vertex_index] = new_vertex[0];
        vertices[vertex_index + 1] = new_vertex[1];
        vertices[vertex_index + 2] = new_vertex[2];
        vertices[vertex_index + 3] = new_vertex[3];
        vertices[vertex_index + 4] = new_vertex[4];
        vertices[vertex_index + 5] = new_vertex[5];
        indices[indices_index] = vertex_index / 6u;
        vertex_index += 6;
      }
      indices_index++;
    }
  }

  vertices = realloc(vertices, vertex_index * sizeof(float));
  indices = realloc(indices, indices_index * sizeof(uint16_t));

  free(stl.bytes);

  return (STL){ header, vertices, indices, vertex_index / 6, indices_index };
}

// void stl_write(STL stl, const char *file_path)
// {
//   size_t buffer_size =
//     STL_HEADER_SIZE + 4 + stl.indices_count / 3 * STL_FACET_SIZE;
//   char *buffer = malloc(buffer_size);

//   // Header
//   memcpy(buffer, stl.header, STL_HEADER_SIZE);
//   size_t buffer_offset = STL_HEADER_SIZE;

//   // Number of facets
//   uint32_t num_of_facets = stl.indices_count / 3u;
//   memcpy(buffer + buffer_offset, &num_of_facets, 4);
//   buffer_offset += 4;

//   // Facets
//   for (size_t facet_index = 0; facet_index < num_of_facets; facet_index++) {
//     memcpy(buffer + buffer_offset,
//            &stl.normals[stl.normal_indices[facet_index] * 3], STL_VERTEX_SIZE);
//     buffer_offset += STL_VERTEX_SIZE;

//     memcpy(buffer + buffer_offset,
//            &stl.vertices[stl.indices[facet_index * 3] * 3], STL_VERTEX_SIZE);
//     buffer_offset += STL_VERTEX_SIZE;

//     memcpy(buffer + buffer_offset,
//            &stl.vertices[stl.indices[facet_index * 3 + 1] * 3],
//            STL_VERTEX_SIZE);
//     buffer_offset += STL_VERTEX_SIZE;

//     memcpy(buffer + buffer_offset,
//            &stl.vertices[stl.indices[facet_index * 3 + 2] * 3],
//            STL_VERTEX_SIZE);
//     buffer_offset += STL_VERTEX_SIZE;

//     memset(buffer + buffer_offset, 0, 2);
//     buffer_offset += 2;
//   }

//   write_file(file_path, buffer, buffer_size);
//   free(buffer);
// }

void stl_free(STL stl)
{
  free(stl.header);
  free(stl.vertices);
  free(stl.indices);
}

ReadFileResult read_file(const char *file_path)
{
  FILE *fp = fopen(file_path, "r");
  if (fp == NULL) {
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
  return (ReadFileResult){ buffer, length };
};

void write_file(const char *file_path, char *buffer, size_t buffer_size)
{
  FILE *fp = fopen(file_path, "w");
  if (fp == NULL) {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fwrite(buffer, 1, buffer_size, fp);

  fclose(fp);
};