#ifndef CC_DAB_HPP
#define CC_DAB_HPP

#include <vector>


extern "C" std::vector<float> cc_dab(const std::vector<float>&inputs, std::vector<float>&state);


constexpr float kp = 1E-3;
constexpr float ki = 1;
constexpr float r_dab = 3.5e-3;
constexpr float l_dc = 10e-6;
constexpr float r_dc = 20e-3;
constexpr float n_ps = 1;
constexpr float r_esr = 5e-3;
constexpr float v_0 = 1000;
constexpr float f_sw = 25e3;
constexpr float l_dab = 2e-6;
constexpr float c_in =  1e-3;
constexpr float c_out =  1e-3;
constexpr float pi =  3.14159265358979323846;
constexpr float sqrt2 = 1.41421356237;
constexpr  float t_sw = 1/f_sw;


float dab_input(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in);
float dab_control(float setpoint, float fb, float en, float *pi_state);
void dab_model(float ps, float v_dab_in, float i_pri_prev, float v_cap_out_past, float i_sec_prev, float *i_sec, float *i_pri);
float dab_output(float i_sec, float i_out, float en, float *v_cap_out);
float dab_input_current(float v_in, float i_in_past, float v_cap_in_past, float i_pri_prev);

#endif //CC_DAB_HPP
