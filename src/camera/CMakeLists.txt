cmake_minimum_required(VERSION 3.16)

add_library(camera STATIC camera.h)

target_include_directories(camera PUBLIC .)

set_target_properties(camera PROPERTIES LINKER_LANGUAGE CXX)