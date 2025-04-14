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
#ifndef OUTPUT_MANAGER_HPP
#define OUTPUT_MANAGER_HPP

#include <utility>
#include <iostream>

#include <nlohmann/json.hpp>
#include "sciplot/sciplot.hpp"

#include "data_model/component.hpp"
#include "data_model/model_output.hpp"
#include "utils/csv_interface.hpp"



class output_manager {
public:
    void set_plot_interval(const std::pair<float, float> &l) {limits = l;}
    void set_output_specs(const std::vector<model_output> &o) {outputs = o;}
    void set_reference(const std::unordered_map<std::string, std::vector<double>> &r){reference_outputs = r;}
    void output_plot(std::vector<double> timebase, std::vector<std::vector<double>> outputs) const;
    void output_data(std::vector<double> timebase, std::vector<std::vector<double>> outputs);
private:
    std::pair<float, float> limits;
    std::vector<model_output> outputs;
    std::unordered_map<std::string, std::vector<double>> reference_outputs;
};



#endif //OUTPUT_MANAGER_HPP
