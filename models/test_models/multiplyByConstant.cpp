 #include "multiplyByConstant.hpp"

std::vector<float> multiply_by_constant(const std::vector<float>&inputs, std::vector<float>&state) {

    std::vector<float> outputs(2, 0);

    // OUTPUT

    #define in inputs[0]
    #define out outputs[0]

    out = in*3;

    return outputs;
}
