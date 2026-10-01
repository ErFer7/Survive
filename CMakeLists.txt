cmake_minimum_required(VERSION 3.14)

project(minimal C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wfatal-errors)

include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(USE_MULTIMEDIA "none" CACHE STRING "" FORCE)
set(USE_DOCTEST OFF CACHE BOOL "" FORCE)
set(USE_PANDOC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  notcurses
  GIT_REPOSITORY git@github.com:dankamongmen/notcurses.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(notcurses)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
set(INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/")

add_executable(survive ${SOURCES})
target_include_directories(survive PUBLIC ${INCLUDE}
                           ${notcurses_SOURCE_DIR}/include)
target_link_libraries(survive PUBLIC notcurses)

option(ENABLE_DEBUG "Enable debug options" OFF)
if (ENABLE_DEBUG)
    target_compile_definitions(survive PUBLIC DEBUG_OPTIONS)
endif()
