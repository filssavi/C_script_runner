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
#include <random>

#include <nlohmann/json.hpp>

#include "metadata_types.hpp"

class model_input {

 public:
    model_input(nlohmann::json input, const std::unordered_map<std::string, std::vector<float>> &d, uint32_t n);
    std::string name;
    input_type type;
    uint8_t input_index;
    float const_value;
    std::vector<float> data;

};



#endif //MODEL_INPUT_HPP
