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
#include "data_model/model_input.hpp"
namespace c_script_engine {
    model_input::model_input(nlohmann::json in, const std::unordered_map<std::string, std::vector<double>>  &d, uint32_t n_steps) {

        if(!in.contains("model_order")) {
            is_overload = true;
        } else {
            input_index = in["model_order"];
        }
        name = in["name"];

        if (in["type"] == "constant") {
            type = constant_input;
            float const_value = in["value"];
            for(auto n = 0; n<n_steps; n++) {
                data.push_back(const_value);
            }
        } else if(in["type"] == "random"){
            type = random_input;
            std::array<float, 2> distribution_parameters = in["distribution"]["parameters"];
            distribution_type_t distribution_type = distribution_type_map[in["distribution"]["name"]];

            std::random_device rd{};
            std::mt19937 gen{rd()};
            if(distribution_type == normal) {

                std::normal_distribution distribution(distribution_parameters[0], distribution_parameters[1]);
                for(auto n = 0; n<n_steps; n++) {
                    data.push_back(distribution(gen));
                }
            } else if (distribution_type == uniform) {

                std::uniform_real_distribution distribution(distribution_parameters[0], distribution_parameters[1]);
                for(auto n = 0; n<n_steps; n++) {
                    data.push_back(distribution(gen));
                }
            }
        } else {
            type = series_input;
            if(!d.contains(name)) {
                std::cout << "Error: input series "<< name << " does not exist!" << std::endl;
                exit(1);
            }
            data = d.at(name);
        }
    }
}