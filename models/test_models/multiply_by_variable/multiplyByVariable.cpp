 #include "multiplyByVariable.hpp"

std::vector<float> multiply_by_variable(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters) {

    std::vector<float> outputs(2, 0);

    // OUTPUT

    #define in inputs[0]
    #define var inputs[1]
    #define out outputs[0]

    out = in*var;

    return outputs;
}
