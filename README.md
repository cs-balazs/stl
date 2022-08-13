# STL

Small library to read STL files.

## Important

Only **binary STL files** are supported for now, and normal vectors are not being loaded.

## Usage

### `stl_read`

- **Input**: path to the `.stl` file.
- **Output**: An `STLReadResult` that represents the loaded verticies.

### `STLReadResult`

#### `verticies`

A `float *` containing all the unique verticies.

#### `indices`

A `uint16_t *` containing indices, that map unique verticies to triangles.  
For example `indices[0]`, `indices[1]` and `indices[2]` describe this triangle: `[ verticies[indices[0]], verticies[indices[1]], verticies[indices[2]] ]`.  
Similarly, `indices[3]`, `indices[4]`, and `indices[5]` describes `[ verticies[indices[3]], verticies[indices[4]], verticies[indices[5]] ]`, and so on.

### `verticies_length`

Number of verticies in `verticies`.  
This means, the **size** of `verticies` is `verticies_length * 3 * sizeof(float)`.

#### `indices_length`

Number of indices in `indices`.  
This means, the **size** of `indices` is `indices * sizeof(uint16_t)`.

## Building

By default the project is build as a library.

> Example build command: `rm -rf build && mkdir build && cd build && cmake .. && make stl && cd ..`

I'm planning to add sime CLI functionality like converting between binary and ASCII STLs, so the project can be configured to build an executable by defining a `STL_BUILD_MAIN` environment variable.

> Example build commands:  
> `export STL_BUILD_MAIN=1`  
> `rm -rf build && mkdir build && cd build && cmake .. && make stl && cd ..`  
> Run the build executable: `./build/stl`

## TODO

- [x] Return the STL header
- [x] Load normal vectors
- [x] Implement `stl_write` (should support both formats, meaning the library could be used as a converter)
- [ ] Support ASCII STL
- [ ] Experiment with custom file formats, similar to binary STL, but using fixed sized (1, 2, 4, or 8 bytes, should be specified in the file's header) indices

## Resources

- https://en.wikipedia.org/wiki/STL_(file_format)
- https://people.sc.fsu.edu/~jburkardt/data/stlb/stlb.html
  - The `cube.stl` test file was taken from this site. The `suzanne.stl` test file is a Blender export (as its header suggests).
- https://people.sc.fsu.edu/~jburkardt/data/stla/stla.html
