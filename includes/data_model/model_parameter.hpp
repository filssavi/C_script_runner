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

#ifndef MODEL_PARAMETER_HPP
#define MODEL_PARAMETER_HPP

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace c_script_engine {

class model_parameter {
public:
    model_parameter() = default;
    explicit model_parameter(const nlohmann::json &json);
    std::string name;
    uint32_t order = 0;
    float value = 0;
};

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(model_parameter, name, order, value);


}

#endif //MODEL_PARAMETER_HPP
