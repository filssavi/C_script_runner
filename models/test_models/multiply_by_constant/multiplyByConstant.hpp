#ifndef BUS_MODEL_HPP
#define BUS_MODEL_HPP

#include <vector>


extern "C" std::vector<float> multiply_by_constant(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters);




#endif //BUS_MODEL_HPP
