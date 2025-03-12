#ifndef SD_DAB_HPP
#define SD_DAB_HPP

#include <vector>


extern "C" std::vector<float> sd_dab(const std::vector<float>&inputs, std::vector<float>&state);


constexpr float kp = 200e-6;
constexpr float ki = 600e-3;
constexpr float r_esr = 3e-3;
constexpr float r_dab = 3.5e-3;
constexpr float l_dc = 5e-6;
constexpr float r_dc = 10e-3;
constexpr float v_pri0 = 1000;
constexpr float v_sec0 = 250;
constexpr float f_sw = 25e3;
constexpr float n_ps = 1000/250;
constexpr float l_dab = 9e-6;
constexpr float c_in = 800e-6;
constexpr float c_out = 800e-6*3;
constexpr float pi = 3.14159265358979323846;

constexpr  float t_sw = 1/f_sw;
constexpr float sqrt2 = 1.41421356237;



float dab_control(float setpoint, float fb, float pi_state, float *pi_state_next);
void dab_model(float ps, float v_dab_in, float v_cap_out_past, float i_pri_prev, float i_sec_prev, float *i_pri, float *i_sec);
float dab_output(float i_sec, float i_out, float v_cap_out_past, float *v_cap_out);
float input_model(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in);
float input_current(float i_in_past, float v_in, float v_cap_in, float i_pri);

#endif //SD_DAB_HPP
