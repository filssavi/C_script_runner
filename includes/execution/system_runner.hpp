

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

#ifndef SYSTEM_RUNNER_HPP
#define SYSTEM_RUNNER_HPP

#include <dlfcn.h>

#include <unordered_set>

#include "utils/builder.hpp"
#include "data_model/multi_component_system.hpp"
#include "data_model/component.hpp"
#include "execution/runner_target.hpp"
#include "execution/interconnect_manger.hpp"
#include "data_model/modules_cache.hpp"
#include "output_manager.hpp"
namespace c_script_engine {
    class system_runner {
    public:
        system_runner(const multi_component_system &sys, modules_cache &c);
        target_cscript_t load_dll(const std::string &path, const std::string &);

        void run_emulation();
        void process_output(const std::string &output_file);
        [[nodiscard]] std::vector<double> get_timebase() const;
        void set_output_type(const output_type &type){out_mgr.out_type = type;}

    private:

        component get_component(const std::string &name);
        interconnect_manger i_m;
        std::unordered_map<std::string, target_cscript_t> targets;
        std::unordered_map<std::string, component> components;
        multi_component_system system;

        std::unordered_map<std::string, std::vector<model_state>> states;
        std::unordered_map<std::string, std::vector<float>> parameters;
        std::unordered_map<std::string, std::vector<runner_input>> inputs;
        std::unordered_map<std::string, std::unordered_map<std::string, std::vector<double>>> outputs;
        modules_cache cache;
        output_manager out_mgr;

        std::unordered_map<std::string, std::pair<std::string, double>> cross_connect;
    };
}


#endif //SYSTEM_RUNNER_HPP
