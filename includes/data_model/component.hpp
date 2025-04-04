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
#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <vector>
#include <cstdint>
#include <iostream>
#include <nlohmann/json.hpp>

#include "data_model/model_input.hpp"
#include "data_model/model_output.hpp"
#include "utils/csv_interface.hpp"

struct target {
    std::string name;
    std::string path;
};

enum output_type {
    plot,
    csv
};


class component {
  public:
    void parse_specifications(const nlohmann::json& specs, const std::string &base_path);

    static void validate_path(const std::string &p);
    static std::string get_full_path(const std::string &filename, const std::string &base_path);

    std::string get_reference_path() {return reference_path; };


    std::string name;
    std::vector<model_input> inputs;
    std::vector<model_output> outputs;
    std::pair<float, float> plot_interval;
    uint32_t n_steps;
    uint32_t sampling_frequency;
    std::vector<float> states;

    target model;
    std::string reference_path;
    output_type out_type;
};



#endif //COMPONENT_HPP
