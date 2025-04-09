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
#include "data_model/multi_component_system.hpp"
#include "data_model/modules_cache.hpp"
#include "execution/runner.hpp"
#include "utils/settings_store.hpp"
#include "utils/builder.hpp"

std::vector<float> get_initial_state(const nlohmann::json& states) {
    std::vector<float> initial_states(states.size());
    for (const auto &s : states) {
        initial_states[s["order"]] = s["initial_value"];
    }
    return initial_states;
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


    if(!target.empty()) {
        std::variant<component, multi_component_system> execution_model;
        if(cache.is_system(target)) {

            auto module = cache.get_system(target);

            nlohmann::json specs;
            std::ifstream spec_stream(module.specs_path);
            spec_stream >> specs;

            multi_component_system sys(specs);
            execution_model = sys;

        } else {

            auto module = cache.get_module(target);

            auto parent = std::filesystem::path(module.specs_path).parent_path().string();
            if (module.needs_rebuilding) {
                builder::build_module(module);
                cache.clear_rebuild_flag(target);
            }

            std::ifstream spec_stream(module.specs_path);

            nlohmann::json specs;
            spec_stream >> specs;

            component comp;
            comp.parse_specifications(specs, parent);
            execution_model = comp;

        }
         run(execution_model, cache);

    }

    return 0;
}
