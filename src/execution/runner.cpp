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

#include "execution/runner.hpp"
namespace c_script_engine {
    void run(
        const std::variant<component, multi_component_system> &model,
        modules_cache &cache,
        const std::string &output_file
        ) {

        output_type out_t;
        if(output_file.empty())
            out_t = plot;
        else
            out_t = csv;

        if(std::holds_alternative<component>(model)) {
            component_runner c(std::get<component>(model), cache);
            c.set_output_type(out_t);
            c.run_emulation();
            c.process_output(output_file);
        } else {
            system_runner s(std::get<multi_component_system>(model), cache);
            s.set_output_type(out_t);
            s.run_emulation();
            s.process_output(output_file);
        }
    }
}
