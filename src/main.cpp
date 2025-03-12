#include <string>
#include "runner.hpp"
#include "sd_dab.hpp"
#include "sciplot/sciplot.hpp"

int main() {

    std::string input_file = "/home/filssavi/git/mea_ring_hil/plecs_data.csv";

    std::string so_path = "/home/filssavi/git/C_script_runner/cmake-build-debug/libsd_dab.so";
    std::string target_name = "sd_dab";

    runner r;
    r.set_target(target_name, so_path);
    r.add_inputs_file(input_file);
    r.add_constant_input("v_in",0, 1000);
    r.add_series_input("i_out", 1, 1);
    r.add_constant_input("v_ref",2, 250);

    r.add_output("v_out", 0, 0);
    r.add_output("i_in", 1, 1);

    const update_model_t fcn = sd_dab;
    r.add_update_fcn(fcn);

    r.set_n_states(6);
    r.set_n_steps(25001);
    r.set_f_sample(25e3);
    r.run_emulation();
    auto outputs = r.get_outputs();

    auto tb = r.get_timebase();


    rapidcsv::Document ref_v = rapidcsv::Document("/home/filssavi/git/mea_ring_hil/reference_out_v.csv", rapidcsv::LabelParams(0, -1));
    rapidcsv::Document ref_i = rapidcsv::Document("/home/filssavi/git/mea_ring_hil/reference_out_i.csv", rapidcsv::LabelParams(0, -1));

    std::vector<float> rev_v_data = ref_v.GetColumn<float>(1);
    std::vector<float> rev_i_data = ref_i.GetColumn<float>(1);

    std::vector ref_data = {rev_v_data, rev_i_data};

    std::vector<std::vector<sciplot::PlotVariant>> plots;

    sciplot::Vec x(tb.data(), tb.size());
    std::pair<float, float> x_ranges = {0.2, 0.3};
    std::vector<std::pair<float, float>> y_ranges = {{240, 260}, {15, 25}};


    for (int i = 0; i<outputs.size(); i++) {
        sciplot::Plot2D p;

        p.drawCurve(x, outputs[i]).lineWidth(1).label("run");
        p.drawCurve(x, ref_data[i]).lineWidth(1).label("Reference");

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




    return 0;
}