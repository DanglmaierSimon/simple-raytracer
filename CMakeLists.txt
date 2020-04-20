cmake_minimum_required(VERSION 3.16)

project(raytracer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(
  -Werror
  -Wall
  -Wextra
  -pedantic
  -Wcast-align
  -Wcast-qual
  -Wctor-dtor-privacy
  -Wdisabled-optimization
  -Wformat=2
  -Winit-self
  -Wlogical-op
  -Wmissing-include-dirs
  -Wnoexcept
  -Wold-style-cast
  -Woverloaded-virtual
  -Wredundant-decls
  -Wshadow
  -Wsign-promo
  -Wstrict-null-sentinel
  -Wundef
  -Wno-unused
  -Wno-variadic-macros
  -Wno-parentheses
  -fdiagnostics-show-option)

add_executable(
  raytracer
  main.cpp
  vec3.h
  ray.h
  hittable.h
  hittable_list.h
  sphere.h
  util.h
  camera.h
  material.h
  lambertian.h
  metal.h
  dielectric.h)
