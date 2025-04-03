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

#ifndef INPUTS_MANAGER_HPP
#define INPUTS_MANAGER_HPP

#include <rapidcsv.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "data_model/model_input.hpp"
#include "utils/csv_interface.hpp"

class inputs_manager {
public:
    inputs_manager(const nlohmann::json &config, const std::string &i_f);
    std::vector<model_input> get_inputs();
private:
    nlohmann::json specs;
    std::string inputs_file;
};



#endif //INPUTS_MANAGER_HPP
