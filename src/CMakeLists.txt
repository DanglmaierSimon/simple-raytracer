add_subdirectory(base)
add_subdirectory(util)
add_subdirectory(materials)
add_subdirectory(objects)
add_subdirectory(benchmarks)
add_subdirectory(external/benchmark)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} util materials base objects)

target_compile_options(
  ${PROJECT_NAME}
  PRIVATE -Wall
          -Werror
          -Wextra
          -pedantic
          -Wcast-align
          -Wcast-qual
          -Wctor-dtor-privacy
          -Wdisabled-optimization
          -Wformat=2
          -Winit-self
          -Wlogical-op
          -Wmissing-include-dirs
          -Wnoexcept
          -Wold-style-cast
          -Woverloaded-virtual
          -Wredundant-decls
          -Wshadow
          -Wsign-promo
          -Wstrict-null-sentinel
          -Wundef
          -Wno-unused
          -Wno-variadic-macros
          -Wno-parentheses
          -Wunused
          -fdiagnostics-show-option)

set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_STANDARD 17)
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CXX_STANDARD_REQUIRED ON)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 "${CMAKE_BINARY_DIR}/bin")
