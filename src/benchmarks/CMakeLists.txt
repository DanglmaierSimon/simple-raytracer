set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(BenchMarkGlassSpheres bm-glass-spheres.cpp)

target_link_libraries(BenchMarkGlassSpheres benchmark::benchmark util materials
                      base objects)

set_target_properties(
  BenchMarkGlassSpheres PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                   "${CMAKE_BINARY_DIR}/bin/benchmarks")

add_executable(BenchMarkManySpheres bm-many-spheres.cpp)

target_link_libraries(BenchMarkManySpheres benchmark::benchmark util materials
                      base objects)

set_target_properties(
  BenchMarkManySpheres PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                  "${CMAKE_BINARY_DIR}/bin/benchmarks")

add_executable(BenchMarkSingleSphere bm-single-sphere.cpp)

target_link_libraries(BenchMarkSingleSphere benchmark::benchmark util materials
                      base objects)

set_target_properties(
  BenchMarkSingleSphere PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                   "${CMAKE_BINARY_DIR}/bin/benchmarks")
