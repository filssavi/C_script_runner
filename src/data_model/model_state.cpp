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


#include "data_model/model_state.hpp"

#include <iostream>


namespace c_script_engine {
    model_state::model_state(nlohmann::json input) {
        name = input["name"];
        is_overload = false;
        if(input.contains("order")) order = input["order"];
        else is_overload =  true;
        if(!input.contains("initial_value")) {
            std::cout << "Error: Initial value not found in the definition of state " << name << std::endl;
            exit(1);
        }
        initial_value = input["initial_value"];
    }

    std::vector<float> model_state::get_state_vector(const std::vector<model_state> &models) {
        std::vector<float> state_vector(models.size());

        for(const auto& model : models) {
            state_vector[ model.order] = model.initial_value;
        }
        return state_vector;
    }
}
