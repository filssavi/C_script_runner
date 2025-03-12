#include "runner.hpp"
#include "sd_dab.hpp"

void runner::add_inputs_file(const std::string& input_file) {
    doc = rapidcsv::Document(input_file, rapidcsv::LabelParams(-1, -1));

    std::string so_path = "/home/filssavi/git/C_script_runner/cmake-build-debug/libsd_dab.so";
    std::string target_name = "sd_dab";
    void* handle = dlopen(so_path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        exit(1);
    }
    dlerror();  // Clear any existing error

    target = (target_cscript_t) dlsym(handle, target_name.c_str());

    if (const char* dlsym_error = dlerror()) {
        std::cerr << "Cannot load symbol: " << dlsym_error << '\n';
        dlclose(handle);
        exit(1);
    }

}

void runner::add_constant_input(const std::string &name, uint8_t input_index, float value) {
    model_input in;
    in.name = name;
    in.input_index = input_index;
    in.value = value;
    in.type = constant_input;
    inputs_metatata.push_back(in);
}

void runner::add_series_input(const std::string &name, uint8_t input_index, uint8_t series_index) {
    model_input in;
    in.name = name;
    in.input_index = input_index;
    in.series_index = series_index;
    in.type = series_input;
    inputs_metatata.push_back(in);
}

void runner::add_output(const std::string &name, uint8_t output_index, uint8_t series_index) {
    model_output out;
    out.name = name;
    out.output_index = output_index;
    out.series_index = series_index;
    outputs_metatata.push_back(out);
}

void runner::add_update_fcn(const update_model_t &f) {
    update_fcn = f;
}

void runner::run_emulation() {

    states = std::vector(n_states, 0.0f);
    outputs = std::vector(outputs_metatata.size(), std::vector<double>());

    for (int i = 0; i<n_steps; i++) {
        std::vector<float> inputs(inputs_metatata.size(), 0);
        for (const auto &in:inputs_metatata) {
            switch (in.type) {
                case constant_input:
                    inputs[in.input_index] = in.value;
                    break;
                case series_input:
                    inputs[in.input_index] = doc.GetCell<float>(in.series_index, i);
                    break;
            }
        }
        auto step_out = target(inputs, states);

        for (const auto &out:outputs_metatata) {
            outputs[out.series_index].push_back(step_out[out.output_index]);
        }
    }
}

std::vector<double> runner::get_timebase() const {
    std::vector<double> timebase(n_steps, 0);
    for (int i = 0; i<n_steps; i++) {
        timebase[i] = i/f_sample;
    }
    return timebase;
}
