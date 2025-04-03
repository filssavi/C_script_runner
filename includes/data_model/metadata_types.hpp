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

#ifndef METADATA_TYPES_HPP
#define METADATA_TYPES_HPP

#include <string>
#include <cstdint>
#include <unordered_map>

enum distribution_type_t {
    normal,
    uniform
};

inline std::unordered_map<std::string, distribution_type_t> distribution_type_map = {
{"normal", normal},
{"uniform", uniform}
};

distribution_type_t name_string_to_distribution_type(std::string);

enum input_type {
    constant_input = 0,
    series_input = 1,
    random_input = 2
};


#endif //METADATA_TYPES_HPP
