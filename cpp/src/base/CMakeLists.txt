set(LIB_NAME base)

set(HEADERS hit_record.h hittable.h hittable_list.h ray.h vec3.h ppm_image.h)

set(SOURCES hittable_list.cpp vec3.cpp ray.cpp)

add_library(${LIB_NAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME} PRIVATE util materials)
