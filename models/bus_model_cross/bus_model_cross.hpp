#ifndef BUS_MODEL_HPP
#define BUS_MODEL_HPP

#include <vector>


struct model_parameters{
  float v_0;
  float p_l;
  int mode;
};

extern "C" std::vector<float> bus_model_cross_source(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters);
extern "C" std::vector<float> bus_model_cross_sink(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters);

std::vector<float> bus_model_cross(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters);
void bus(float i_out, float v_in, int fault, float &v_out, float &i_in, float &v_cross, float &i_cross, const model_parameters &params);




#endif //BUS_MODEL_HPP
