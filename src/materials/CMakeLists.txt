cmake_minimum_required(VERSION 3.16)

add_library(materials STATIC dielectric.h lambertian.h metal.h)

target_include_directories(materials PUBLIC .)

set_target_properties(materials PROPERTIES LINKER_LANGUAGE CXX)