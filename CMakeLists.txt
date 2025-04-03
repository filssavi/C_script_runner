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
        src/data_model/model_input.cpp
        src/data_model/model_output.cpp
        src/utils/csv_interface.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/dependencies)


add_executable(C_script_runner ${SOURCES})

target_link_libraries(C_script_runner PRIVATE
        CLI11::CLI11
        rapidcsv::rapidcsv
        implot::implot
        nlohmann_json::nlohmann_json
)