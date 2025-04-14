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

#include "execution/component_runner.hpp"

component_runner::component_runner(const component &c, modules_cache &cache) {
    comp = c;
    out_mgr.set_plot_interval(c.plot_interval);
    out_mgr.set_output_specs(c.outputs);
    out_mgr.set_reference(csv_interface::parse_file(comp.get_reference_path()));

    auto metadata = cache.get_module_metadata(comp.name);
    if (metadata.needs_rebuilding) {
        builder::build_module(metadata);
        cache.clear_rebuild_flag(comp.name);
    }

}

void component_runner::run_emulation() {

    load_target();
    outputs = std::vector(comp.outputs.size(), std::vector<double>());

    for (int i = 0; i<comp.n_steps; i++) {
        std::vector<float> inputs(comp.inputs.size(), 0);
        for (const auto &in:comp.inputs) {
            inputs[in.input_index] = in.data[i];
        }

        auto step_out = target(inputs, comp.states);

        for (const auto &out:comp.outputs) {
            outputs[out.series_index].push_back(step_out[out.output_index]);
        }
    }

}

std::vector<double> component_runner::get_timebase() const {
    std::vector<double> timebase(comp.n_steps, 0);
    float sampling_time = 1.f/comp.sampling_frequency;
    for (int i = 0; i<comp.n_steps; i++) {
        timebase[i] = i*sampling_time;
    }
    return timebase;
}

void component_runner::load_target() {
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

void component_runner::process_output() {
    if (comp.out_type == plot) {
        out_mgr.output_plot(get_timebase(), outputs);
    } else if (comp.out_type == csv) {
        out_mgr.output_data(get_timebase(), outputs);
    }
}
