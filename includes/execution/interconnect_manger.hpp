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
#include <algorithm>
#include <iostream>

#include "data_model/system_metadata.hpp"
namespace c_script_engine {
    class interconnect_manger {
    public:
        interconnect_manger();
        void add_connection(const endpoint_descriptor& src, const endpoint_descriptor& dst, double initial_value);
        void update_value(const endpoint_descriptor &ep, double value);
        double get_value(const endpoint_descriptor& ep);
        bool is_overridden(const endpoint_descriptor& ep);
        bool is_overriding(const endpoint_descriptor& ep);
    private:

        struct interconect {
            endpoint_descriptor source;
            endpoint_descriptor destination;
            double value = 0;
        };

        std::vector<interconect> connections;
    };
}



#endif //INTERCONNECT_MANGER_HPP
