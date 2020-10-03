set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_NAME materials)

add_library(${LIB_NAME} SHARED dielectric.h lambertian.h material.h metal.h)

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${LIB_NAME} PUBLIC .)
