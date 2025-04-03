//
// Created by filssavi on 3/12/25.
//

#ifndef METADATA_TYPES_HPP
#define METADATA_TYPES_HPP

#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <array>

enum distribution_type_t {
    normal,
    uniform
};

inline std::unordered_map<std::string, distribution_type_t> distribution_type_map = {
{"normal", normal},
{"uniform", uniform}
};
distribution_type_t name_string_to_distribution_type(std::string);

enum input_type {
    constant_input = 0,
    series_input = 1,
    random_input = 2
};

struct model_input {
    std::string name;
    input_type type;
    uint8_t input_index;
    float const_value;
    std::vector<float> series_values;
    std::array<float, 2> distribution_parameters;
    distribution_type_t distribution_type;
};

struct model_output {
    std::string name;
    uint8_t output_index;
    uint8_t series_index;
};


#endif //METADATA_TYPES_HPP
