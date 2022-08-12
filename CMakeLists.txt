cmake_minimum_required(VERSION 3.10)
project(stl VERSION 1.0)

if(DEFINED ENV{STL_BUILD_MAIN})
  add_executable(${PROJECT_NAME} src/main.c src/stl.c src/stl.h)
else()
  add_library(${PROJECT_NAME} STATIC src/stl.h src/stl.c)
  target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
endif()
