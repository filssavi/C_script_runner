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


runner::runner(const component &c) : out_mgr(c){
    comp = c;
}

void runner::run_emulation() {
    load_target();
    outputs = std::vector(comp.outputs.size(), std::vector<double>());

    for (int i = 0; i<comp.n_steps; i++) {
        std::vector<float> inputs(comp.inputs.size(), 0);
        for (const auto &in:comp.inputs) {
            switch (in.type) {
                case constant_input:
                    inputs[in.input_index] = in.const_value;
                    break;
                case series_input:
                case random_input:
                    inputs[in.input_index] = in.data[i];
                    break;
            }
        }
        states = comp.states;
        auto step_out = target(inputs, states);

        for (const auto &out:comp.outputs) {
            outputs[out.series_index].push_back(step_out[out.output_index]);
        }
    }

    out_mgr.set_timebase(get_timebase());
    out_mgr.set_outputs(outputs);
}

std::vector<double> runner::get_timebase() const {
    std::vector<double> timebase(comp.n_steps, 0);
    float sampling_time = 1.f/comp.sampling_frequency;
    for (int i = 0; i<comp.n_steps; i++) {
        timebase[i] = i*sampling_time;
    }
    return timebase;
}

void runner::load_target() {
    void* handle = dlopen(comp.model.path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        exit(1);
    }
    dlerror();  // Clear any existing error

    target = (target_cscript_t) dlsym(handle, comp.model.name.c_str());

    if (const char* dlsym_error = dlerror()) {
        std::cerr << "Cannot load symbol: " << dlsym_error << '\n';
        dlclose(handle);
        exit(1);
    }
}

void runner::process_output() {
    if (comp.out_type == plot) {
        out_mgr.output_plot();
    } else if (comp.out_type == csv) {
        out_mgr.output_data();
    }
}
