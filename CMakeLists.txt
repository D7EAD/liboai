cmake_minimum_required(VERSION 3.13)

project(liboai)

IF(WIN32)
    set(VCPKG_CMAKE_PATH $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE FILEPATH "Location of vcpkg.cmake")
    include(${VCPKG_CMAKE_PATH})
    find_package(ZLIB REQUIRED)
    find_package(nlohmann_json CONFIG REQUIRED)
    find_package(CURL REQUIRED)
ENDIF()

option(BUILD_EXAMPLES "Build example applications" OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(liboai)

if(BUILD_EXAMPLES)
  add_subdirectory(documentation)
endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT oai)
