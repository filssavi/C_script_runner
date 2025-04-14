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

#include "execution/interconnect_manger.hpp"

interconnect_manger::interconnect_manger(): connections() {
}

void interconnect_manger::add_connection(const endpoint_descriptor& src, const endpoint_descriptor& dst, const double initial_value) {
    connections[interconnect_to_string(src, dst)] = initial_value;
}

void interconnect_manger::update_value(const endpoint_descriptor& src, const endpoint_descriptor& dst, const double value) {
    if(!connections.contains(interconnect_to_string(src, dst))) {
        std::cout << "Error: Attempted update of unknown connection" << std::endl;
        exit(1);
    }
    connections[interconnect_to_string(src, dst)] = value;
}

double interconnect_manger::get_value(const endpoint_descriptor& src, const endpoint_descriptor& dst) {
    if(!connections.contains(interconnect_to_string(src, dst))) {
        std::cout << "Error: Attempted retrival of unknown connection" << std::endl;
        exit(1);
    }
    return connections[interconnect_to_string(src, dst)];
}

std::string interconnect_manger::interconnect_to_string(const endpoint_descriptor &src, const endpoint_descriptor &dst) {
    return src.to_string() + "---" + dst.to_string();
}
