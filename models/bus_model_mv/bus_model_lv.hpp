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

extern "C" std::vector<float> bus_model_lv(const std::vector<float>&inputs, std::vector<float>&state);
void bus(float i_out, float v_in, int fault, float &v_out, float &i_in, model_parameters &params);




#endif //BUS_MODEL_HPP
