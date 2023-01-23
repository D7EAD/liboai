cmake_minimum_required(VERSION 3.10)

project(oai)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)

add_library(oai
components/completions.cpp
components/edits.cpp
components/embeddings.cpp
components/files.cpp
components/fine_tunes.cpp
components/images.cpp
components/models.cpp
components/moderations.cpp
core/authorization.cpp
core/response.cpp
)

target_compile_features(oai PRIVATE cxx_std_17)

target_link_libraries(oai PRIVATE cpr::cpr)
target_link_libraries(oai PRIVATE nlohmann_json::nlohmann_json)

install(TARGETS oai DESTINATION lib)
install(FILES liboai.h DESTINATION include)
install(FILES
include/components/completions.h
include/components/edits.h
include/components/embeddings.h
include/components/files.h
include/components/fine_tunes.h
include/components/images.h
include/components/models.h
include/components/moderations.h
DESTINATION include/components)
install(FILES
include/core/authorization.h
include/core/exception.h
include/core/network.h
include/core/response.h
DESTINATION include/core)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT oai)
