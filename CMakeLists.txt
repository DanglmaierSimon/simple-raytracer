project(Raytracer)

set(CMAKE_CXX_STANDARD 17)

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
