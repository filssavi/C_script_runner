//  Copyright 2025 Filippo Savi <filssavi@gmail.com>
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>
#include <filesystem>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include "data_model/component.hpp"
#include "data_model/modules_cache.hpp"
#include "utils/settings_store.hpp"
#include "runner.hpp"

std::vector<float> get_initial_state(const nlohmann::json& states) {
    std::vector<float> initial_states(states.size());
    for (const auto &s : states) {
        initial_states[s["order"]] = s["initial_value"];
    }
    return initial_states;
}

void compile(const std::filesystem::path &file) {
    const std::string file_path ="lib" +file.stem().string() + std::string(".so");
    if (std::filesystem::exists(file_path)) {
        std::filesystem::remove_all(file_path);
    }
    std::vector<std::string> compile_command_args = {
        "g++",
        "-fPIC",
        "-shared",
        file.string(),
        "-o",
        file_path
    };
    std::string command;
    for (auto &p:compile_command_args) command += p + " ";
    std::system(command.c_str());
}

void run(std::istream &spec_stream,const std::string &base_path) {

    nlohmann::json specs;
    spec_stream >> specs;

    component comp;
    comp.parse_specifications(specs, base_path);

    runner r(comp);
    r.run_emulation();
    r.process_output();

}
int main(int argc, char **argv) {

    CLI::App app{"General purpose runner for C-script derived functions"};

    std::string target;
    app.add_option("Module", target, "Component or System name");
    CLI11_PARSE(app, argc, argv);

    modules_cache cache;

    if(!cache.contains(target)) {
        std::cout << "Module " << target << " not found" << std::endl;
        exit(1);
    }

    auto module = cache.get_module(target);

    if(!target.empty()) {

        auto parent = std::filesystem::path(module.specs_path).parent_path().string();

        std::string current_dir = std::filesystem::current_path().string();
        std::filesystem::current_path(parent);

        if (module.needs_rebuilding) {
            compile(std::filesystem::path(module.target_path));
        }

        std::ifstream spec_stream(module.specs_path);
        run(spec_stream, parent);

        std::filesystem::current_path(current_dir);
    }

    return 0;
}
