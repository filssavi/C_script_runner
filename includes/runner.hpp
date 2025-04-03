#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <cstdint>
#include <string>
#include <utility>
#include <vector>
#include "rapidcsv.h"
#include <dlfcn.h>

#include "metadata_types.hpp"
#include "random_input_generator.hpp"

typedef std::vector<float> (*target_cscript_t)(std::vector<float>, std::vector<float>&);
typedef std::function< std::vector<float>(std::vector<float>, std::vector<float>&)> update_model_t;



class runner {
public:
    void add_inputs(const std::vector<model_input> &in);
    void add_outputs(const std::vector<model_output> &out) {outputs_metatata = out;}

    void initialize_states(std::vector<float> s) {states = std::move(s);}

    void set_f_sample(const float f){f_sample = f;};
    void set_n_steps(const int n) {n_steps = n;}
    void run_emulation();
    std::vector<std::vector<double>> get_outputs() {return outputs;}
    [[nodiscard]] std::vector<double> get_timebase() const;

    void set_target(const std::string &n, const std::string &p);
private:
    random_input_generator input_generator;

    target_cscript_t target;

    std::vector<model_input> inputs_metatata;
    std::vector<model_output> outputs_metatata;
    uint32_t n_steps = 1e3;

    std::vector<std::vector<double>> outputs;
    std::vector<float> states;
    float f_sample;

};



#endif //RUNNER_HPP
