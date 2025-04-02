#ifndef BUS_MODEL_HPP
#define BUS_MODEL_HPP

#include <vector>


struct model_parameters{
  float t_sw;
  float c;
  float r_in;
  float r_esr;
  float v_0;
  float i_l;
};

extern "C" std::vector<float> bus_model(const std::vector<float>&inputs, std::vector<float>&state);





#endif //BUS_MODEL_HPP
