cmake_minimum_required(VERSION 3.16)

add_library(shapes STATIC hittable_list.h hittable.h sphere.h)

target_include_directories(shapes PUBLIC .)

set_target_properties(shapes PROPERTIES LINKER_LANGUAGE CXX)