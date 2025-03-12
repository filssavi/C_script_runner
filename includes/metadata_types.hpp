//
// Created by filssavi on 3/12/25.
//

#ifndef METADATA_TYPES_HPP
#define METADATA_TYPES_HPP

#include <string>
#include <cstdint>
#include <vector>

enum input_type {
    constant_input = 0,
    series_input = 1
};

struct model_input {
    std::string name;
    input_type type;
    uint8_t input_index;
    float const_value;
    std::vector<float> series_values;
};

struct model_output {
    std::string name;
    uint8_t output_index;
    uint8_t series_index;
};


#endif //METADATA_TYPES_HPP
