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
#include "data_model/metadata_types.hpp"
#include "data_model/multi_component_system.hpp"
#include "data_model/modules_cache.hpp"
#include "execution/runner.hpp"
#include "utils/settings_store.hpp"
#include "utils/builder.hpp"

int main(int argc, char **argv) {

    CLI::App app{"General purpose runner for C-script derived functions"};

    std::string target;
    std::string output_path;
    app.add_option("Module", target, "Component or System name");
    app.add_option("--output_file", output_path, "Path for the output csv file");
    CLI11_PARSE(app, argc, argv);

    c_script_engine::modules_cache cache;

    if(!cache.contains(target)) {
        std::cout << "Module " << target << " not found" << std::endl;
        exit(1);
    }


    if(!target.empty()) {
        std::variant<c_script_engine::component, c_script_engine::multi_component_system> execution_model;
        if(cache.is_system(target)) {

            auto module = cache.get_system_metadata(target);

            c_script_engine::multi_component_system sys(module.specs_path, cache);
            execution_model = sys;

        } else {

            auto module = cache.get_module_metadata(target);

            c_script_engine::component comp(module.specs_path);
            execution_model = comp;

        }
         run(execution_model, cache, output_path);

    }

    return 0;
}
