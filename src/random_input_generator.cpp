

#include "random_input_generator.hpp"

void random_input_generator::set_type(const std::string &name, distribution_type_t t, std::array<float,2> p) {
    types[name] = t;
    parameters[name] = p;
}

float random_input_generator::get_value(const std::string &name) {
    if(types[name] == uniform) {
        std::uniform_real_distribution distribution(parameters[name][0], parameters[name][1]);
        return distribution(gen);
    }else if(types[name] == normal) {
        std::normal_distribution distribution(parameters[name][0], parameters[name][1]);
        auto val =distribution(gen);
        return val;
    } else return 0.0;
}
