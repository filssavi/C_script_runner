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

#include "output_manager.hpp"

namespace c_script_engine{
    void output_manager::output_plot(std::vector<double> timebase, std::unordered_map<std::string, std::vector<double>> output_values) const {

        std::vector<std::vector<sciplot::PlotVariant>> plots;

        sciplot::Vec x(timebase.data(), timebase.size());
        for(auto &[out_name, out_data]:output_values) {
            sciplot::Plot2D p;

            auto trace_name = out_name;
            std::replace(trace_name.begin(), trace_name.end(), '_', ' ');

            p.drawCurve(x, out_data).lineWidth(1).label( trace_name+"_{run}");

            auto opt_output = model_output::get_output_by_name(outputs, out_name);
            if(!opt_output.has_value()) {
                continue;
            }
            const auto& output = opt_output.value();
            if(reference_outputs.contains(output.name)) {
                auto ref = reference_outputs.at(output.name);
                p.drawCurve(x, ref).lineWidth(1).label(trace_name+ "_{ref}");
            } else {
                std::cout << "Reference data for " << output.name << " not found" << std::endl;
            }
            auto y_min = output.y_range.first;
            auto y_max = std::max<float>(output.y_range.second, 1);
            p.yrange(y_min,y_max);
            p.xrange(limits.first, limits.second);
            plots.push_back({p});
        }


        sciplot::Figure f(plots);



        sciplot::Canvas canvas = {{f}};
        // Set color palette for all Plots that do not have a palette set (plot2) / the default palette
        canvas.defaultPalette("set1");
        // Set canvas output size
        canvas.size(1720, 880);

        canvas.show();

    }

    void output_manager::output_data(std::vector<double> timebase, std::unordered_map<std::string, std::vector<double>> output_values, const std::string &file_path) {
        std::vector<std::pair<std::string, std::vector<double>>> data;
        data.emplace_back("timebase", timebase);

        for (auto &[out_name, out_data]:output_values) {
            data.emplace_back(out_name, out_data);
        }
        csv_interface::write_file(file_path, data);
    }
}

