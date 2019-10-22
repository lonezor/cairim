cmake_minimum_required(VERSION 3.0)

project(cairim)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(XLIB REQUIRED x11)

add_executable(cairim
  src/event_loop.cpp
  src/cairo_image_surface.cpp
  src/scene/scene.cpp
  src/scene/object.cpp
  src/rendering_context.cpp
  src/cairo_xlib_window.cpp
  src/timer.cpp
  src/main.cpp
)

target_link_libraries(cairim
${CAIRO_LIBRARIES}
${XLIB_LIBRARIES}
)
target_include_directories(cairim
PUBLIC
${CAIRO_INCLUDE_DIRS}
src
src/scene
)
target_compile_options(cairim PUBLIC ${CAIRO_CFLAGS_OTHER})

