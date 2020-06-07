set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_NAME util)

add_library(${LIB_NAME} STATIC elapsed_timer.h profiler.h util.h)

target_include_directories(${LIB_NAME} PUBLIC .)

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
