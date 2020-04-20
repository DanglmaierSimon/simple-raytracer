cmake_minimum_required(VERSION 3.16)

add_library(geometry STATIC ray.h vec3.h)

target_link_libraries(geometry PUBLIC util)

target_include_directories(geometry PUBLIC .)

set_target_properties(geometry PROPERTIES LINKER_LANGUAGE CXX)