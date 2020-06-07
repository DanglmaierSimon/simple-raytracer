set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_NAME util)

set(HEADERS elapsed_timer.h profiler.h util.h)
set(SOURCES util.cpp profiler.cpp)

add_library(${LIB_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories(${LIB_NAME} PUBLIC .)

target_link_libraries(${LIB_NAME} PRIVATE base)
