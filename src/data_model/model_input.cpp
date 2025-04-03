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

model_input::model_input(nlohmann::json in, const rapidcsv::Document &doc) {

    name = in["name"];
    input_index = in["model_order"];

    if (in["type"] == "constant") {
        type = constant_input;
        const_value = in["value"];
    } else if(in["type"] == "random"){
        type = random_input;
        distribution_parameters = in["distribution"]["parameters"];
        distribution_type = distribution_type_map[in["distribution"]["name"]];
    } else {
        type = series_input;
        const uint8_t idx = in["series_order"];
        series_values = doc.GetColumn<float>(idx);
    }

}
