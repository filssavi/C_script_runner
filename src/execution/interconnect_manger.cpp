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
namespace c_script_engine {
    interconnect_manger::interconnect_manger(): connections() {
    }

    void interconnect_manger::add_connection(const endpoint_descriptor& src, const endpoint_descriptor& dst, const double initial_value) {
        connections.emplace_back(src, dst, initial_value);
    }

    void interconnect_manger::update_value(const endpoint_descriptor &ep, const double value) {
        for(auto &ic:connections) {
            if(ic.source.component == ep.component && ic.source.port== ep.port) {
                ic.value = value;
            }
        }
    }


    double interconnect_manger::get_value(const endpoint_descriptor& ep) {
        for(const auto& ic:connections) {
            if(ic.destination.component == ep.component && ic.destination.port == ep.port) {
                return ic.value;
            }
        }
        std::cout << "Error: Attempted get the value of unknown connection" << std::endl;
        exit(1);
    }

    bool interconnect_manger::is_overridden(const endpoint_descriptor &ep) {
        return std::ranges::any_of(
            connections.begin(),
            connections.end(),
            [&](const auto& ic) {
                return ic.destination.component == ep.component && ic.destination.port == ep.port;
            });
    }

    bool interconnect_manger::is_overriding(const endpoint_descriptor &ep) {
        return std::ranges::any_of(
        connections.begin(),
        connections.end(),
        [&](const auto& ic) {
            return ic.source.component == ep.component && ic.source.port == ep.port;
        });
    }
}