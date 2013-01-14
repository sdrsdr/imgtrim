cmake_minimum_required(VERSION 2.6)
include(FindPkgConfig)

project(imgtrim)

pkg_search_module(MC REQUIRED MagickCore)

if(MC_FOUND)
  message(STATUS "MagickCore found! :) libs: ${MC_LIBRARIES}  incs:  ${MC_INCLUDE_DIRS} cfl:${MC_CFLAGS_OTHER}")
else(MC_FOUND)
  message(FATAL_ERROR "MagickCore NOT found! :(")
endif(MC_FOUND)

add_executable(imgtrim main.c)
SET(CMAKE_C_FLAGS "${MC_CFLAGS_OTHER} -Wall -pedantic -std=c99")
INCLUDE_DIRECTORIES( ${MC_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(imgtrim ${MC_LIBRARIES})

install(TARGETS imgtrim RUNTIME DESTINATION bin)
