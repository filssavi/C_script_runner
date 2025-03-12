#include <string>
#include "runner.hpp"
#include "sd_dab.hpp"
#include "sciplot/sciplot.hpp"

int main() {

    std::string input_file = "/home/filssavi/git/mea_ring_hil/plecs_data.csv";

    runner r;
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
    auto out = r.get_outputs();

    auto tb = r.get_timebase();


    rapidcsv::Document ref_v = rapidcsv::Document("/home/filssavi/git/mea_ring_hil/reference_out_v.csv", rapidcsv::LabelParams(0, -1));
    rapidcsv::Document ref_i = rapidcsv::Document("/home/filssavi/git/mea_ring_hil/reference_out_i.csv", rapidcsv::LabelParams(0, -1));


    std::vector<float> rev_v_data = ref_v.GetColumn<float>(1);
    std::vector<float> rev_i_data = ref_i.GetColumn<float>(1);

    sciplot::Vec x(tb.data(), tb.size());
    sciplot::Vec y1(out[0].data(), out[0].size());
    sciplot::Vec y2(out[1].data(), out[1].size());
    sciplot::Plot2D plot_v;
    plot_v.drawCurve(x, y1).lineWidth(1).label("run");
    plot_v.drawCurve(x, rev_v_data).lineWidth(1).label("Reference");
    plot_v.yrange(240,260);
    plot_v.xrange(0.2, 0.3);

    sciplot::Plot2D plot_i;
    plot_i.drawCurve(x, y2).lineWidth(1).label("run");
    plot_i.drawCurve(x, rev_i_data).lineWidth(1).label("Reference");

    plot_i.yrange(15,25);
    plot_i.xrange(0.2, 0.3);

    sciplot::Figure figure = {{plot_v}, {plot_i}};
    sciplot::Canvas canvas = {{figure}};
    // Set color palette for all Plots that do not have a palette set (plot2) / the default palette
    canvas.defaultPalette("set1");
    // Set canvas output size
    canvas.size(1720, 880);

    canvas.show();


    return 0;
}