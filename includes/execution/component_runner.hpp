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
#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <dlfcn.h>

#include "data_model/model_input.hpp"
#include "data_model/model_output.hpp"
#include "data_model/component.hpp"
#include "output_manager.hpp"

typedef std::vector<float> (*target_cscript_t)(std::vector<float>, std::vector<float>&);
typedef std::function< std::vector<float>(std::vector<float>, std::vector<float>&)> update_model_t;



class component_runner {
public:
    explicit component_runner(const component &c);
    void run_emulation();
    [[nodiscard]] std::vector<double> get_timebase() const;
    void load_target();
    void process_output();
private:
    component comp;
    target_cscript_t target;

    std::vector<std::vector<double>> outputs;
    std::vector<float> states;
    output_manager out_mgr;
};



#endif //RUNNER_HPP
