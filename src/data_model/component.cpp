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

        if (!comp["model"].contains("target_name")) {
            std::cout<<"Target name not specified"<<std::endl;
            exit(1);
        }
        if (!comp["model"].contains("target_path")) {
            std::cout<<"Target path not specified"<<std::endl;
            exit(1);
        }

        std::string target_path = comp["model"]["target_path"];

        auto base_path = path.parent_path().string();
        model.path = base_path + "/lib" + std::filesystem::path(comp["model"]["target_path"]).replace_extension().string() + ".so";


        model.name = comp["model"]["target_name"];

        name = comp["model"]["target_name"];
        sampling_frequency = comp["model"]["sampling_frequency"];
        n_steps = comp["run_length"];

        for (const auto &s : comp["states"]) {
            auto dbg = s.dump(4);
            states.emplace_back(s);
        }

        const auto inputs_path = get_full_path(comp["inputs"]["series_file"], base_path);

        auto input_data = csv_interface::parse_file(inputs_path);
        for (auto &in:comp["inputs"]["specs"]) {
            model_input i(in, input_data, comp["run_length"]);
            inputs.push_back(i);
        }


        reference_path = get_full_path(comp["reference_outputs"], base_path);
        for (auto &out:comp["outputs"]["specs"]) {
            model_output o(out);
            outputs.emplace_back(o);

        }

        plot_interval = {comp["plot_interval"][0],comp["plot_interval"][1]};

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
}