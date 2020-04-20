add_compile_options(
  -Werror
  -Wall
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
  -fdiagnostics-show-option)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(raytracer main.cpp)

add_subdirectory(geometry)
add_subdirectory(camera)
add_subdirectory(materials)
add_subdirectory(shapes)
add_subdirectory(util)

target_link_libraries(raytracer PUBLIC util shapes materials geometry camera)

target_include_directories(raytracer PUBLIC .)