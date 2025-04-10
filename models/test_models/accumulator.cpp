 #include "accumulator.hpp"
#include <iostream>

std::vector<float> accumulator(const std::vector<float>&inputs, std::vector<float>&state) {

    std::vector<float> outputs(2, 0);

    // OUTPUT

    #define in inputs[0]

    #define out outputs[0]

    #define mem state[0]
    mem += in;
    out = mem;

    return outputs;
}
