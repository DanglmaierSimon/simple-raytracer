set(LIB_NAME objects)

set(HEADERS camera.h sphere.h)

set(SOURCES camera.cpp)

add_library(${LIB_NAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME} PRIVATE base util)
