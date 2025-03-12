#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "rapidcsv.h"
#include <dlfcn.h>

enum input_type {
    constant_input = 0,
    series_input = 1
};

struct model_input {
    std::string name;
    input_type type;
    uint8_t input_index;
    float value;
    uint8_t series_index;
};

struct model_output {
    std::string name;
    uint8_t output_index;
    uint8_t series_index;
};

typedef std::vector<float> (*target_cscript_t)(std::vector<float>, std::vector<float>&);
typedef std::function< std::vector<float>(std::vector<float>, std::vector<float>&)> update_model_t;

class runner {
public:
    void add_inputs_file(const std::string& input_file);
    void add_constant_input(const std::string &name, uint8_t input_index, float value);
    void add_series_input(const std::string &name, uint8_t input_index, uint8_t series_index);

    void add_output(const std::string &name, uint8_t output_index, uint8_t series_index);
    void add_update_fcn(const update_model_t &f);

    void set_n_states(const uint8_t n){n_states = n;}
    void set_f_sample(const float f){f_sample = f;};
    void set_n_steps(const int n) {n_steps = n;}
    void run_emulation();
    std::vector<std::vector<double>> get_outputs() {return outputs;}
    [[nodiscard]] std::vector<double> get_timebase() const;

    void set_target(std::string n, std::string p){target_name = std::move(n); target_path = std::move(p);};
private:
    std::string target_path;
    std::string target_name;

    target_cscript_t target;
    update_model_t update_fcn;

    std::vector<model_input> inputs_metatata;
    std::vector<model_output> outputs_metatata;
    rapidcsv::Document doc;
    uint8_t n_states = 0;
    uint32_t n_steps = 1e3;

    std::vector<std::vector<double>> outputs;
    std::vector<float> states;
    float f_sample;

};



#endif //RUNNER_HPP
