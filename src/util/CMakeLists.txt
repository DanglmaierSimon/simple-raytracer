cmake_minimum_required(VERSION 3.16)

add_library(util STATIC util.h)

target_include_directories(util PUBLIC .)

set_target_properties(util PROPERTIES LINKER_LANGUAGE CXX)