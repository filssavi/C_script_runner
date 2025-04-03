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
#ifndef RANDOM_INPUT_GENERATOR_HPP
#define RANDOM_INPUT_GENERATOR_HPP
#include <random>
#include <array>
#include <map>
#include "data_model/metadata_types.hpp"


class random_input_generator {
public:

  void set_type(const std::string &name, distribution_type_t t, std::array<float,2> p);
  float get_value(const std::string &name);
private:
  std::map<std::string, distribution_type_t> types;
  std::map<std::string, std::array<float, 2>> parameters;
  std::random_device rd{};
  std::mt19937 gen{rd()};
};



#endif //RANDOM_INPUT_GENERATOR_HPP
