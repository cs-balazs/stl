# STL

Small helper library to handle STL files.  
The way data is loaded is currently designed to be used for OpenGL's `GL_ELEMENT_ARRAY_BUFFER`.

> Only **binary STL files** are supported for now

## TODO

- [x] Return the STL header
- [x] Load normal vectors
- [x] Implement `stl_write` for binary _.stl_ files
- [ ] Support ASCII STL
- [ ] Implement `stl_write` for ASCII _.stl_ files
- [ ] Support loading into a `{ float *vertices; float *normals; size_t vertices_count }`
  > normals_count = vertices_count / 3

## Resources

- https://en.wikipedia.org/wiki/STL_(file_format)
- https://people.sc.fsu.edu/~jburkardt/data/stlb/stlb.html
  - The `cube.stl` test file was taken from this site. The `suzanne.stl` test file is a Blender export (as its header suggests).
- https://people.sc.fsu.edu/~jburkardt/data/stla/stla.html
