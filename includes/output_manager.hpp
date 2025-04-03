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

#include <nlohmann/json.hpp>
#include "sciplot/sciplot.hpp"
#include <rapidcsv.h>
#include <utility>
#include "data_model/model_output.hpp"
#include "data_model/metadata_types.hpp"

class output_manager {
public:
    explicit output_manager(nlohmann::json s,const std::string &ref_path);
    void set_outputs(std::vector<std::vector<double>> o) {outputs = std::move(o);}
    void set_timebase(std::vector<double> tb) {timebase = std::move(tb);}
    void output_plot() const;
    void output_data();
    [[nodiscard]] std::vector<model_output> get_outputs();
private:

    std::pair<float, float> x_ranges;
    std::vector<std::pair<float, float>> y_ranges;

    nlohmann::json specs;
    std::vector<double> timebase;
    std::vector<std::vector<double>> outputs;
    rapidcsv::Document reference_outputs;
};



#endif //OUTPUT_MANAGER_HPP
