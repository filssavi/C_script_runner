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

#ifndef INTERCONNECT_MANGER_HPP
#define INTERCONNECT_MANGER_HPP

#include <unordered_map>
#include <vector>
#include <iostream>

#include "data_model/system_metadata.hpp"

class interconnect_manger {
public:
    interconnect_manger();
    void add_connection(const endpoint_descriptor& src, const endpoint_descriptor& dst, double initial_value);
    void update_value(const endpoint_descriptor& src, const endpoint_descriptor& dst, double value);
    double get_value(const endpoint_descriptor& src, const endpoint_descriptor& dst);
private:
    static std::string interconnect_to_string(const endpoint_descriptor& src, const endpoint_descriptor& dst);
    std::unordered_map<std::string, double> connections;
};



#endif //INTERCONNECT_MANGER_HPP
