

// Copyright 2025 Filippo Savi <filssavi@gmail.com>
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "utils/builder.hpp"

void builder::build_module(const component_metadata &md) {

    auto parent = std::filesystem::path(md.specs_path).parent_path().string();
    std::string current_dir = std::filesystem::current_path().string();
    std::filesystem::current_path(parent);

    compile(std::filesystem::path(md.target_path));

    std::filesystem::current_path(current_dir);
}

void builder::compile(std::filesystem::path path) {
    const std::string file_name ="lib" +path.stem().string() + std::string(".so");
    if (std::filesystem::exists(file_name)) {
        std::filesystem::remove_all(file_name);
    }
    std::vector<std::string> compile_command_args = {
        "g++",
        "-fPIC",
        "-shared",
        path.string(),
        "-o",
        file_name
    };
    std::string command;
    for (auto &p:compile_command_args) command += p + " ";
    std::system(command.c_str());
}
