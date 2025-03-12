cmake_minimum_required(VERSION 3.30)
project(C_script_runner)

set(CMAKE_CXX_STANDARD 23)

find_package(rapidcsv REQUIRED)
find_package(implot REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CLI11 REQUIRED)


set(SOURCES /
        src/main.cpp
        src/runner.cpp
        src/output_manager.cpp
        src/inputs_manager.cpp
        includes/inputs_manager.hpp
        includes/metadata_types.hpp
)

include_directories(${CMAKE_SOURCE_DIR}/includes)

add_library(sd_dab SHARED)
target_sources(sd_dab PUBLIC src/sd_dab.cpp )
set_target_properties(sd_dab PROPERTIES LINKER_LANGUAGE CXX)

add_executable(C_script_runner ${SOURCES})

target_link_libraries(C_script_runner PRIVATE
        CLI11::CLI11
        rapidcsv::rapidcsv
        implot::implot
        nlohmann_json::nlohmann_json
)

add_dependencies( C_script_runner sd_dab)
