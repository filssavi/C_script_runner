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
#include "data_model/model_output.hpp"
namespace c_script_engine {
    model_output::model_output(const nlohmann::json &out) {
        name = out["name"];
        output_index = out["model_order"];
        y_range = {out["plot_range"][0], out["plot_range"][1]};
    }

    std::optional<model_output> model_output::get_output_by_name(std::vector<model_output> vect, const std::string &name)  {
        for(const auto& out : vect) {
            if(out.name == name) return out;
        }
        return {};
    }
}
