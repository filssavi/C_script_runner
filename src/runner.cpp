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

#include "runner.hpp"


void runner::add_inputs(const std::vector<model_input> &in) {
    inputs_metatata = in;
    for(auto &i: inputs_metatata) {
        if(i.type == random_input) {
            input_generator.set_type(i.name, i.distribution_type, i.distribution_parameters);
        }
    }
}

void runner::run_emulation() {

    outputs = std::vector(outputs_metatata.size(), std::vector<double>());

    for (int i = 0; i<n_steps; i++) {
        std::vector<float> inputs(inputs_metatata.size(), 0);
        for (const auto &in:inputs_metatata) {
            switch (in.type) {
                case constant_input:
                    inputs[in.input_index] = in.const_value;
                    break;
                case series_input:
                    inputs[in.input_index] = in.series_values[i];
                    break;
                case random_input:
                    inputs[in.input_index] = input_generator.get_value(in.name);
                    break;
            }
        }
        auto step_out = target(inputs, states);

        for (const auto &out:outputs_metatata) {
            outputs[out.series_index].push_back(step_out[out.output_index]);
        }
    }
}

std::vector<double> runner::get_timebase() const {
    std::vector<double> timebase(n_steps, 0);
    for (int i = 0; i<n_steps; i++) {
        timebase[i] = i/f_sample;
    }
    return timebase;
}

void runner::set_target(const std::string &target_name, const std::string &target_path) {
    void* handle = dlopen(target_path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        exit(1);
    }
    dlerror();  // Clear any existing error

    target = (target_cscript_t) dlsym(handle, target_name.c_str());

    if (const char* dlsym_error = dlerror()) {
        std::cerr << "Cannot load symbol: " << dlsym_error << '\n';
        dlclose(handle);
        exit(1);
    }
}
