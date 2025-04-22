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

#ifndef MODEL_OUTPUT_HPP
#define MODEL_OUTPUT_HPP

#include <string>
#include <cstdint>
#include <nlohmann/json.hpp>
namespace c_script_engine {
    class model_output {
    public:
        explicit model_output(const nlohmann::json &out);
        model_output(std::string n, std::pair<float, float> r){name = n; y_range = r;}
        std::string name;
        uint8_t output_index{};
        uint8_t series_index{};
        std::pair<float, float> y_range;

    };
}



#endif //MODEL_OUTPUT_HPP
