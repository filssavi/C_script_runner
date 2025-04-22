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
    void output_manager::output_plot(std::vector<double> timebase, std::vector<std::vector<double>> output_values) const {

        std::vector<std::vector<sciplot::PlotVariant>> plots;

        sciplot::Vec x(timebase.data(), timebase.size());

        for (int i = 0; i<output_values.size(); i++) {
            sciplot::Plot2D p;

            p.drawCurve(x, output_values[i]).lineWidth(1).label("run");
            std::string name = outputs[i].name;
            if(reference_outputs.contains(name)) {
                auto ref = reference_outputs.at(name);
                p.drawCurve(x, ref).lineWidth(1).label("Reference");
            } else {
                std::cout << "Reference data for " << name << " not found" << std::endl;
            }

            p.yrange(outputs[i].y_range.first,outputs[i].y_range.second);
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

    void output_manager::output_data(std::vector<double> timebase, std::vector<std::vector<double>> output_values) {
        std::vector<std::pair<std::string, std::vector<double>>> data;
        data.emplace_back("timebase", timebase);

        for (int i = 0; i<output_values.size(); i++) {
            data.emplace_back(outputs[i].name, output_values[i]);
        }
        csv_interface::write_file("results.csv", data);
    }
}

