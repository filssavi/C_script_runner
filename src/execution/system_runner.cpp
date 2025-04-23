

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

#include "execution/system_runner.hpp"
namespace c_script_engine {
    system_runner::system_runner(const multi_component_system &sys, modules_cache &c) : system(sys){

        cache = c;

        for(auto &component_inst:sys.components) {
            if(!c.contains(component_inst.type)) {
                std::cout << "Error: component "<< component_inst.type << "not found" <<std::endl;
            }
            auto component_metadata = c.get_module_metadata(component_inst.type);
            components[component_inst.name] = c.get_component(component_inst.type);
            if (component_metadata.needs_rebuilding) {
                builder::build_module(component_metadata);
                c.clear_rebuild_flag(component_metadata.name);
            }

            auto path =  std::filesystem::path(component_metadata.target_path);
            auto base_path = path.parent_path().string();
            auto exec_path = base_path + "/lib" + path.filename().replace_extension().string() + ".so";
            targets[component_inst.name] = load_dll(exec_path, component_metadata.name);

            component comp(component_metadata.specs_path);
            for(auto s:comp.states){
                states[component_inst.name].push_back(s);
            }

            for(auto &i:comp.inputs) {
                bool overridden = false;
                for(auto &ov:sys.inputs_overloads) {
                    auto split_point = ov.name.find('.');
                    auto instance = ov.name.substr(0, split_point);
                    auto port = ov.name.substr(split_point + 1);
                    if(component_inst.name== instance) {
                        if(i.name == port) {
                            inputs[component_inst.name].emplace_back(ov.data, i.input_index);
                            overridden = true;
                        }
                    }
                }
                if(!overridden) {
                    inputs[component_inst.name].emplace_back(i.data, i.input_index);
                }
            }

        }
        for(const auto &[source, destination]:sys.connections) {
            i_m.add_connection(source, destination, 0);
        }
        for(auto &o:sys.outputs_overloads) {
            outputs[o.component][o.port] = std::vector<double>(sys.n_steps, 0);
        }

        system = sys;
    }

    void system_runner::run_emulation() {

        for (int current_step = 0; current_step<system.n_steps; current_step++) {
            for(auto &c:system.components) {
                std::vector<float> input_values;
                for(auto &in:components[c.name].inputs) {
                    endpoint_descriptor ep = {c.name ,in.name, in.input_index};
                    if(i_m.is_overridden(ep)) {
                        input_values.push_back(i_m.get_value(ep));
                    }else {
                        try {
                            auto input = runner_input::get_input_at_position(inputs[c.name], in.input_index, current_step);
                            input_values.push_back(input);
                        } catch(std::runtime_error &e) {
                            std::cout << "Cant Find input " << in.name << " at index: " << std::to_string(in.input_index) << " for component " << c.name << std::endl;
                            std::exit(1);
                        }
                    }
                }

                auto step_out = targets[c.name](input_values, states[c.name]);
                for(const auto &out:components[c.name].outputs) {
                    i_m.update_value({c.name, out.name, out.output_index}, step_out[out.output_index]);
                    for(auto & sys_out: outputs[c.name]) {
                        if(sys_out.first == out.name) {
                            auto out_val = step_out[out.output_index];
                            sys_out.second[current_step] = out_val;
                        }
                    }
                }
            }
        }
    }

    void system_runner::process_output(const std::string &output_file) {

        std::vector<std::vector<double>> raw_outputs;
        std::vector<model_output> specs;
        for(auto &[module, outputs_map] :outputs) {
            for(auto &[port, data]: outputs_map) {
                raw_outputs.push_back(data);
                std::string out_name  = module + "." + port;
                std::pair<float, float> range =  {0, *std::ranges::max_element(data)*1.1};
                specs.emplace_back(out_name,range);
            }
        }
        auto tb = get_timebase();
        out_mgr.set_plot_interval({tb.front(), tb.back()});
        out_mgr.set_output_specs(specs);
        if (out_mgr.out_type == plot) {
            out_mgr.output_plot(get_timebase(), raw_outputs);
        } else if (out_mgr.out_type == csv) {
            out_mgr.output_data(get_timebase(), raw_outputs, output_file);
        }
    }

    std::vector<double> system_runner::get_timebase() const {
        std::vector<double> timebase(system.n_steps, 0);
        float sampling_time = 1.f/system.sampling_frequency;
        for (int i = 0; i<system.n_steps; i++) {
            timebase[i] = i*sampling_time;
        }
        return timebase;
    }

    target_cscript_t system_runner::load_dll(const std::string &path, const std::string &module_name) {
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << "Cannot open library: " << dlerror() << '\n';
            exit(1);
        }
        dlerror();  // Clear any existing error

        target_cscript_t t = (target_cscript_t) dlsym(handle, module_name.c_str());

        if (const char* dlsym_error = dlerror()) {
            std::cerr << "Cannot load symbol: " << dlsym_error << '\n';
            dlclose(handle);
            exit(1);
        }
        return t;
    }
}