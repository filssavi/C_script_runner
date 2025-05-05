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
#include "data_model/component.hpp"
namespace c_script_engine {
    component::component(const std::filesystem::path &path) {
        std::ifstream spec_stream(path);

        nlohmann::json comp;
        spec_stream >> comp;

        auto base_path = path.parent_path().string();
        construct_component(comp,comp["run_length"], base_path);

    }

    component::component(const std::filesystem::path &path, uint32_t n_steps_override) {

        std::ifstream spec_stream(path);

        nlohmann::json comp;
        spec_stream >> comp;

        auto base_path = path.parent_path().string();
        construct_component(comp, n_steps_override, base_path);
    }

    void component::construct_component(const nlohmann::json &specs, uint32_t n_steps_override, const std::string &base_path) {

        if (!specs["model"].contains("target_name")) {
            std::cout<<"Target name not specified"<<std::endl;
            exit(1);
        }
        if (!specs["model"].contains("target_path")) {
            std::cout<<"Target path not specified"<<std::endl;
            exit(1);
        }

        std::string target_path = specs["model"]["target_path"];

        model.path = base_path + "/lib" + std::filesystem::path(specs["model"]["target_path"]).replace_extension().string() + ".so";


        model.name = specs["model"]["target_name"];

        name = specs["model"]["target_name"];
        sampling_frequency = specs["model"]["sampling_frequency"];
        n_steps = n_steps_override;

        for (const auto &s : specs["states"]) {
            auto dbg = s.dump(4);
            states.emplace_back(s);
        }

        const auto inputs_path = get_full_path(specs["inputs"]["series_file"], base_path);

        auto input_data = csv_interface::parse_file(inputs_path);
        for (auto &in:specs["inputs"]["specs"]) {
            model_input i(in, input_data, n_steps);
            inputs.push_back(i);
        }


        reference_path = get_full_path(specs["reference_outputs"], base_path);
        for (auto &out:specs["outputs"]["specs"]) {
            model_output o(out);
            outputs.emplace_back(o);
        }

        for (auto &param:specs["parameters"]) {
            model_parameter p(param);
            parameters.emplace_back(p);
        }

        plot_interval = {specs["plot_interval"][0],specs["plot_interval"][1]};
    }


    void component::validate_path(const std::string &p) {
        if (!std::filesystem::exists(p)) {
            std::cerr << "Required file does not exist: " << p << std::endl;
            exit(1);
        }
        if (std::filesystem::is_directory(p)) {
            std::cerr << "Required file is a directory: " << p << std::endl;
            exit(1);
        }
    }
    std::string component::get_full_path(const std::string &filename, const std::string &base_path) {
        std::string path;
        if(!path.starts_with(".") || path.starts_with("/")) {
            path =base_path + "/" + filename;
        } else {
            path = std::filesystem::canonical(filename);
        }
        return path;
    }

    std::vector<float> component::get_parameters(const std::vector<model_parameter> &overloads) const {
        std::vector<float> out(parameters.size());

        std::set<uint32_t> orders;
        for (const auto &p : parameters) {
            if (p.order >= parameters.size()) {
                std::cerr << "Parameter order out of bounds: " << p.order << std::endl;
                exit(1);
            }
            if (!orders.insert(p.order).second) {
                std::cerr << "Duplicate parameter order: " << p.order << std::endl;
                exit(1);
            }
            out[p.order] = p.value;
            for(const auto &o : overloads) {
                if(o.name == p.name) out[p.order] = o.value;
            }
        }

        if (orders.size() != parameters.size()) {
            std::cerr << "Non-consecutive parameter orders" << std::endl;
            exit(1);
        }

        return out;
    }
}
