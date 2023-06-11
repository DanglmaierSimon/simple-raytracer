set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(raytracer-benchmark benchmarks.cpp)

target_link_libraries(raytracer-benchmark benchmark::benchmark util materials
                      base objects)

set_target_properties(raytracer-benchmark PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                     "${CMAKE_BINARY_DIR}/bin")
