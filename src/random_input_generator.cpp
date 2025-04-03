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
#include "random_input_generator.hpp"

void random_input_generator::set_type(const std::string &name, distribution_type_t t, std::array<float,2> p) {
    types[name] = t;
    parameters[name] = p;
}

float random_input_generator::get_value(const std::string &name) {
    if(types[name] == uniform) {
        std::uniform_real_distribution distribution(parameters[name][0], parameters[name][1]);
        return distribution(gen);
    }else if(types[name] == normal) {
        std::normal_distribution distribution(parameters[name][0], parameters[name][1]);
        auto val =distribution(gen);
        return val;
    } else return 0.0;
}
