cmake_minimum_required(VERSION 3.13)

include(CMakePackageConfigHelpers)

project(oai VERSION 4.0.1)

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

find_package(nlohmann_json CONFIG REQUIRED)
find_package(CURL REQUIRED)

add_library(${PROJECT_NAME})

function(make_absolute_paths result_var)
    set(absolute_paths)
    foreach(file IN LISTS ARGN)
        list(APPEND absolute_paths "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
    endforeach()
    set(${result_var} "${absolute_paths}" PARENT_SCOPE)
endfunction()

set(HEADERS_RELATIVE
  "include/liboai.h"
)

make_absolute_paths(HEADERS ${HEADERS_RELATIVE})
source_group("include" FILES ${HEADERS})

set(COMPONENT_HEADERS_RELATIVE
  "include/components/audio.h"
  "include/components/azure.h"
  "include/components/chat.h"
  "include/components/completions.h"
  "include/components/edits.h"
  "include/components/embeddings.h"
  "include/components/files.h"
  "include/components/fine_tunes.h"
  "include/components/images.h"
  "include/components/models.h"
  "include/components/moderations.h"
)

make_absolute_paths(COMPONENT_HEADERS ${COMPONENT_HEADERS_RELATIVE})
source_group("include/components" FILES ${COMPONENT_HEADERS})

set(COMPONENT_SOURCES_RELATIVE
  "components/audio.cpp"
  "components/azure.cpp"
  "components/chat.cpp"
  "components/completions.cpp"
  "components/edits.cpp"
  "components/embeddings.cpp"
  "components/files.cpp"
  "components/fine_tunes.cpp"
  "components/images.cpp"
  "components/models.cpp"
  "components/moderations.cpp"
)

make_absolute_paths(COMPONENT_SOURCES ${COMPONENT_SOURCES_RELATIVE})
source_group("source/components" FILES ${COMPONENT_SOURCES})

set(CORE_HEADERS_RELATIVE
  "include/core/authorization.h"
  "include/core/exception.h"
  "include/core/netimpl.h"
  "include/core/network.h"
  "include/core/response.h"
)

make_absolute_paths(CORE_HEADERS ${CORE_HEADERS_RELATIVE})
source_group("include/core" FILES ${CORE_HEADERS})

set(CORE_SOURCES_RELATIVE
  "core/authorization.cpp"
  "core/netimpl.cpp"
  "core/response.cpp"
)

make_absolute_paths(CORE_SOURCES ${CORE_SOURCES_RELATIVE})
source_group("source/core" FILES ${CORE_SOURCES})

target_sources(${PROJECT_NAME}
  PRIVATE
    ${COMPONENT_SOURCES}
    ${CORE_SOURCES}
  PUBLIC
    "$<BUILD_INTERFACE:${HEADERS}>"
    "$<BUILD_INTERFACE:${COMPONENT_HEADERS}>"
    "$<BUILD_INTERFACE:${CORE_HEADERS}>"
    "$<INSTALL_INTERFACE:${HEADERS_RELATIVE}>"
    "$<INSTALL_INTERFACE:${COMPONENT_HEADERS_RELATIVE}>"
    "$<INSTALL_INTERFACE:${CORE_HEADERS_RELATIVE}>"
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    nlohmann_json::nlohmann_json
    CURL::libcurl
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<INSTALL_INTERFACE:include>"
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib EXPORT ${PROJECT_NAME}Targets)
install(FILES ${HEADERS} DESTINATION "include")
install(FILES ${COMPONENT_HEADERS} DESTINATION "include/components")
install(FILES ${CORE_HEADERS} DESTINATION "include/core")

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "lib/cmake/${PROJECT_NAME}"
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  COMPATIBILITY AnyNewerVersion
)

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE oai::
  DESTINATION "lib/cmake/${PROJECT_NAME}"
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  DESTINATION "lib/cmake/${PROJECT_NAME}"
)
