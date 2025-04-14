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

#ifndef SYSTEM_METADATA_HPP
#define SYSTEM_METADATA_HPP

#include <string>
#include <cstdint>

struct endpoint_descriptor {
    std::string component;
    std::string port;
    uint32_t order;

    [[nodiscard]] std::string to_string() const {
        return component + "." + port + "." + std::to_string(order);
    }

    friend bool operator==(const endpoint_descriptor &lhs, const endpoint_descriptor &rhs) {
        return lhs.component == rhs.component
               && lhs.port == rhs.port
               && lhs.order == rhs.order;
    }

    friend bool operator!=(const endpoint_descriptor &lhs, const endpoint_descriptor &rhs) {
        return !(lhs == rhs);
    }
};

struct system_connection {
    endpoint_descriptor source;
    endpoint_descriptor destination;
};

struct system_component {
    std::string name;
    std::string type;
    float sampling_frequency;
};


#endif //SYSTEM_METADATA_HPP
