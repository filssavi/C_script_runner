

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

#ifndef RUNNER_TARGET_HPP
#define RUNNER_TARGET_HPP

#include <vector>
#include <functional>

typedef std::vector<float> (*target_cscript_t)(std::vector<float>, std::vector<float>&);
typedef std::function< std::vector<float>(std::vector<float>, std::vector<float>&)> update_model_t;

struct runner_input {
    std::vector<double> data;
    uint32_t position;
    static std::vector<double> get_input_at_position(const std::vector<runner_input> &inputs, uint32_t position);
};

inline std::vector<double> runner_input::get_input_at_position(const std::vector<runner_input> &inputs, uint32_t pos) {
    for(const auto &[data, position]:inputs){
        if(position == pos) {
            return data;
        };
    }
    throw std::runtime_error("runner_input::get_input_at_position: position out of range");
}
#endif //RUNNER_TARGET_HPP
