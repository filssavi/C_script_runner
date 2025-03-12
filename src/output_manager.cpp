#include "output_manager.hpp"

#include <utility>

std::vector<model_output> output_manager::get_outputs() const {
    std::vector<model_output> outputs;
    for (auto &out:specs["outputs"]["specs"]) {
        model_output o;
        o.name = out["name"];
        o.output_index = out["model_order"];
        o.series_index = out["reference_order"];
        outputs.emplace_back(o);
    }
    return outputs;
}

output_manager::output_manager(nlohmann::json s) {
    specs = std::move(s);
    const auto reference_outputs_path = std::filesystem::canonical(specs["reference_outputs"]);
    reference_outputs = rapidcsv::Document(reference_outputs_path, rapidcsv::LabelParams(0, -1));

}

void output_manager::output_plot() const {

    std::vector<std::vector<sciplot::PlotVariant>> plots;

    sciplot::Vec x(timebase.data(), timebase.size());
    std::pair<float, float> x_ranges = {0.2, 0.3};
    std::vector<std::pair<float, float>> y_ranges = {{240, 260}, {15, 25}};


    for (int i = 0; i<outputs.size(); i++) {
        sciplot::Plot2D p;

        p.drawCurve(x, outputs[i]).lineWidth(1).label("run");
        std::vector<float> ref = reference_outputs.GetColumn<float>(i+1);
        p.drawCurve(x, ref).lineWidth(1).label("Reference");

        p.yrange(y_ranges[i].first,y_ranges[i].second);
        p.xrange(x_ranges.first, x_ranges.second);
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

void output_manager::output_data() {
}

