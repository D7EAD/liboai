cmake_minimum_required(VERSION 3.13)

project(liboai)

option(BUILD_EXAMPLES "Build example applications" OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(liboai)

if(BUILD_EXAMPLES)
  add_subdirectory(documentation)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT oai)