#ifndef CC_DAB_HPP
#define CC_DAB_HPP

#include <vector>

struct model_parameters{
  float kp;
  float ki;
  float r_dab;
  float l_dc;
  float r_dc;
  float n_ps;
  float r_esr;
  float v_0;
  float f_sw;
  float l_dab;
  float c_in;
  float c_out;
  float pi;
  float sqrt2;
  float t_sw;
};


extern "C" std::vector<float> cc_dab(const std::vector<float>&inputs, std::vector<float>&state);



float dab_input(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in, struct model_parameters *p);
float dab_control(float setpoint, float fb, float en, float *pi_state, struct model_parameters *p);
void dab_model(float ps, float v_dab_in, float i_pri_prev, float v_cap_out_past, float i_sec_prev, float *i_sec, float *i_pri, struct model_parameters *p);
float dab_output(float i_sec, float i_out, float en, float *v_cap_out, struct model_parameters *p);
float dab_input_current(float v_in, float i_in_past, float v_cap_in_past, float i_pri_prev, struct model_parameters *p);

#endif //CC_DAB_HPP
