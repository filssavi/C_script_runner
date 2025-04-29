cmake_minimum_required(VERSION 3.30)
project(C_script_runner)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(CLI11 REQUIRED)


set(SOURCES /
        src/main.cpp
        src/output_manager.cpp
        src/data_model/model_input.cpp
        src/data_model/model_output.cpp
        src/data_model/component.cpp
        src/data_model/modules_cache.cpp
        src/data_model/model_state.cpp
        src/data_model/multi_component_system.cpp
        src/execution/system_runner.cpp
        src/execution/interconnect_manger.cpp
        src/execution/runner.cpp
        src/execution/component_runner.cpp
        src/utils/builder.cpp
        src/utils/csv_interface.cpp
        src/utils/settings_store.cpp
        src/data_model/model_parameter.cpp
        src/data_model/model_parameter.hpp

)

include_directories(${CMAKE_SOURCE_DIR}/includes)
include_directories(${CMAKE_SOURCE_DIR}/dependencies)


add_executable(C_script_runner ${SOURCES})

target_link_libraries(C_script_runner PRIVATE
        openssl::openssl
        CLI11::CLI11
        tomlplusplus::tomlplusplus
        nlohmann_json::nlohmann_json
)