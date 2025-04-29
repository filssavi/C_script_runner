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

//
// Created by filssavi on 4/29/25.
//

#include "../../includes/data_model/model_parameter.hpp"

namespace c_script_engine {
    model_parameter::model_parameter(const nlohmann::json &json) {
        name = json.at("name");
        value = json.at("value");
        if(json.contains("order")) order = json.at("order");
    }
} // c_script_engine