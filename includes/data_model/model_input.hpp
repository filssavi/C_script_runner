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

#ifndef MODEL_INPUT_HPP
#define MODEL_INPUT_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <array>
#include <iostream>
#include <random>

#include <nlohmann/json.hpp>

#include "metadata_types.hpp"
namespace c_script_engine {
    class model_input {
    public:
        model_input() = default;
        model_input(nlohmann::json input, const std::unordered_map<std::string, std::vector<double>> &d, uint32_t n);
        std::string name = "";
        input_type type = constant_input;
        uint8_t input_index = 99;
        std::vector<double> data{};
        bool is_overload = false;

    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(model_input, name, type, input_index, data);
}

#endif //MODEL_INPUT_HPP
