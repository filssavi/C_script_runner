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
#include <set>
#include <iostream>
#include <nlohmann/json.hpp>

#include "data_model/model_state.hpp"
#include "data_model/model_parameter.hpp"
#include "data_model/model_input.hpp"
#include "data_model/model_output.hpp"
#include "utils/csv_interface.hpp"

struct target {
    std::string name;
    std::string path;
};


namespace c_script_engine {
    class component {
    public:
        component() = default;
        component(const std::filesystem::path &path);
        component(const std::filesystem::path &path, uint32_t n_steps_override);
        void construct_component(const nlohmann::json& specs, uint32_t n_steps_override, const std::string &base_path);

        static void validate_path(const std::string &p);
        static std::string get_full_path(const std::string &filename, const std::string &base_path);

        std::string get_reference_path() {return reference_path;};

        std::vector<float> get_parameters(const std::vector<model_parameter> &overloads) const;

        std::string name;
        std::vector<model_input> inputs;
        std::vector<model_parameter> parameters;
        std::vector<model_output> outputs;
        std::pair<float, float> plot_interval;
        uint32_t n_steps;
        uint32_t sampling_frequency;
        std::vector<model_state> states;

        target model;
        std::string reference_path;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(component, name, inputs, outputs,parameters, plot_interval, n_steps, sampling_frequency, states);
}



#endif //COMPONENT_HPP
