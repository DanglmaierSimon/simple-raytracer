set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_NAME base)

set(HEADERS camera.h hit_record.h hittable.h hittable_list.h ray.h sphere.h vec3.h)

set(SOURCES camera.cpp hittable_list.cpp vec3.cpp ray.cpp)

add_library(${LIB_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME} PRIVATE util materials)


