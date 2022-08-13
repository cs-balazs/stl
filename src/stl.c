#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stl.h"

typedef struct ReadFileResult
{
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

  float *vertices = malloc(facets * 3 * STL_VERTEX_SIZE);
  float *normals = malloc(facets * STL_VERTEX_SIZE);
  uint16_t *indices = malloc(facets * 3 * sizeof(uint16_t));
  uint16_t *normals_indices = malloc(facets * sizeof(uint16_t));
  size_t vertex_index = 0ul;
  size_t indices_index = 0ul;
  size_t normals_index = 0ul;
  size_t normals_indices_index = 0ul;

  for (size_t i = 84; i < stl.length; i += STL_FACET_SIZE)
  {

    float new_normal[3];
    memcpy(new_normal, stl.bytes + i, STL_VERTEX_SIZE);

    uint16_t normal_duplicate_index = 0;
    for (size_t prev_normal_index = 0; prev_normal_index < normals_index;
         prev_normal_index += 3)
    {
      if (normals[prev_normal_index] == new_normal[0] &&
          normals[prev_normal_index + 1] == new_normal[1] &&
          normals[prev_normal_index + 2] == new_normal[2])
      {
        normal_duplicate_index = prev_normal_index;
        break;
      }
    }

    if (normal_duplicate_index > 0)
    {
      normals_indices[normals_indices_index] = normal_duplicate_index / 3u;
    }
    else
    {
      normals[normals_index] = new_normal[0];
      normals[normals_index + 1] = new_normal[1];
      normals[normals_index + 2] = new_normal[2];
      normals_indices[normals_indices_index] = normals_index / 3u;
      normals_index += 3;
    }
    normals_indices_index++;

    for (size_t vertex_offset = i + STL_VERTEX_SIZE; vertex_offset <= i + 3 * STL_VERTEX_SIZE;
         vertex_offset += STL_VERTEX_SIZE)
    {
      float new_vertex[3];
      memcpy(new_vertex, stl.bytes + vertex_offset, STL_VERTEX_SIZE);

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

  vertices = realloc(vertices, vertex_index * sizeof(float));
  indices = realloc(indices, indices_index * sizeof(uint16_t));
  normals = realloc(normals, normals_index * sizeof(float));
  normals_indices = realloc(normals_indices, normals_indices_index * sizeof(uint16_t));

  free(stl.bytes);

  return (STL){header, vertices, indices, vertex_index / 3,
               indices_index, normals, normals_indices};
}

void stl_write(STL stl, const char *file_path)
{
  size_t buffer_size = STL_HEADER_SIZE + 4 + stl.indices_count / 3 * STL_FACET_SIZE;
  char *buffer = malloc(buffer_size);

  // Header
  memcpy(buffer, stl.header, STL_HEADER_SIZE);
  size_t buffer_offset = STL_HEADER_SIZE;

  // Number of facets
  uint32_t num_of_facets = stl.indices_count / 3u;
  memcpy(buffer + buffer_offset, &num_of_facets, 4);
  buffer_offset += 4;

  // Facets
  for (size_t facet_index = 0; facet_index < num_of_facets; facet_index++)
  {
    memcpy(buffer + buffer_offset, &stl.normals[stl.normal_indices[facet_index] * 3], STL_VERTEX_SIZE);
    buffer_offset += STL_VERTEX_SIZE;

    memcpy(buffer + buffer_offset, &stl.vertices[stl.indices[facet_index * 3] * 3], STL_VERTEX_SIZE);
    buffer_offset += STL_VERTEX_SIZE;

    memcpy(buffer + buffer_offset, &stl.vertices[stl.indices[facet_index * 3 + 1] * 3], STL_VERTEX_SIZE);
    buffer_offset += STL_VERTEX_SIZE;

    memcpy(buffer + buffer_offset, &stl.vertices[stl.indices[facet_index * 3 + 2] * 3], STL_VERTEX_SIZE);
    buffer_offset += STL_VERTEX_SIZE;

    memset(buffer + buffer_offset, 0, 2);
    buffer_offset += 2;
  }

  write_file(file_path, buffer, buffer_size);
  free(buffer);
}

void stl_free(STL stl)
{
  free(stl.header);
  free(stl.vertices);
  free(stl.indices);
  free(stl.normals);
  free(stl.normal_indices);
}

ReadFileResult read_file(const char *file_path)
{
  FILE *fp = fopen(file_path, "r");
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

void write_file(const char *file_path, char *buffer, size_t buffer_size)
{
  FILE *fp = fopen(file_path, "w");
  if (fp == NULL)
  {
    perror("Error while opening the file.\n");
    exit(EXIT_FAILURE);
  }

  fwrite(buffer, 1, buffer_size, fp);

  fclose(fp);
};