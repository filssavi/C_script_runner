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
#include "data_model/component.hpp"

void component::parse_specifications(const nlohmann::json &specs, const std::string &base_path) {

    if (!specs["model"].contains("target_name")) {
        std::cout<<"Target name not specified"<<std::endl;
        exit(1);
    }
    if (!specs["model"].contains("target_path")) {
        std::cout<<"Target path not specified"<<std::endl;
        exit(1);
    }

    model.path = get_full_path(specs["model"]["target_path"], base_path);
    validate_path(model.path);

    model.name = specs["model"]["target_name"];

    name = specs["model"]["target_name"];
    sampling_frequency = specs["model"]["sampling_frequency"];
    n_steps = specs["run_length"];

    states = std::vector<float>(specs["states"].size());

    for (const auto &s : specs["states"]) {
        states[s["order"]] = s["initial_value"];
    }

    const auto inputs_path = get_full_path(specs["inputs"]["series_file"], base_path);
    validate_path(inputs_path);

    auto input_data = csv_interface::parse_file(inputs_path);
    for (auto &in:specs["inputs"]["specs"]) {
        model_input i(in, input_data, specs["run_length"]);
        inputs.push_back(i);
    }


    reference_path = get_full_path(specs["reference_outputs"], base_path);
    validate_path(reference_path);
    for (auto &out:specs["outputs"]["specs"]) {
        model_output o(out);
        outputs.emplace_back(o);

    }


}

void component::validate_path(const std::string &p) {
    if (!std::filesystem::exists(p)) {
        std::cerr << "Required file does not exist: " << p << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(p)) {
        std::cerr << "Required file is a directory: " << p << std::endl;
        exit(1);
    }
}
std::string component::get_full_path(const std::string &filename, const std::string &base_path) {
    std::string path;
    if(!path.starts_with(".") || path.starts_with("/")) {
        path =base_path + "/" + filename;
    } else {
        path = std::filesystem::canonical(filename);
    }
    return path;
}
