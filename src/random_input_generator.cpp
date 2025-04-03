

#include "random_input_generator.hpp"

void random_input_generator::set_type(distribution_type t, std::vector<float> p) {
    type = t;
    parameters = p;
}

float random_input_generator::get_value() {
    if(type == uniform) {
        std::normal_distribution distribution(parameters[0], parameters[1]);
        return distribution(gen);
    }else if(type == normal) {
        std::uniform_real_distribution distribution(parameters[0], parameters[1]);
        return distribution(gen);
    } else return 0.0;
}
