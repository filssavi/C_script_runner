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
#include "../includes/inputs_manager.hpp"

inputs_manager::inputs_manager(const nlohmann::json &config, const std::string &i_f) {
    specs = config;
    inputs_file = i_f;
}

std::vector<model_input> inputs_manager::get_inputs() {

    const rapidcsv::Document doc = rapidcsv::Document(inputs_file, rapidcsv::LabelParams(0, -1));
    auto input_data = csv_interface::parse_file(inputs_file);
    std::vector<model_input> ret_val;
    for (auto &in:specs["inputs"]["specs"]) {
        model_input i(in, input_data, specs["run_length"]);
        ret_val.push_back(i);
    }
    return ret_val;
}
