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
#include "rapidcsv.h"
#include <dlfcn.h>

#include "data_model/model_input.hpp"
#include "data_model/model_output.hpp"
#include "random_input_generator.hpp"

typedef std::vector<float> (*target_cscript_t)(std::vector<float>, std::vector<float>&);
typedef std::function< std::vector<float>(std::vector<float>, std::vector<float>&)> update_model_t;



class runner {
public:
    void add_inputs(const std::vector<model_input> &in);
    void add_outputs(const std::vector<model_output> &out) {outputs_metatata = out;}

    void initialize_states(std::vector<float> s) {states = std::move(s);}

    void set_f_sample(const float f){f_sample = f;};
    void set_n_steps(const int n) {n_steps = n;}
    void run_emulation();
    std::vector<std::vector<double>> get_outputs() {return outputs;}
    [[nodiscard]] std::vector<double> get_timebase() const;

    void set_target(const std::string &n, const std::string &p);
private:
    random_input_generator input_generator;

    target_cscript_t target;

    std::vector<model_input> inputs_metatata;
    std::vector<model_output> outputs_metatata;
    uint32_t n_steps = 1e3;

    std::vector<std::vector<double>> outputs;
    std::vector<float> states;
    float f_sample;

};



#endif //RUNNER_HPP
