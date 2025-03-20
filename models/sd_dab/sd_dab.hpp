#ifndef SD_DAB_HPP
#define SD_DAB_HPP

#include <vector>


extern "C" std::vector<float> sd_dab(const std::vector<float>&inputs, std::vector<float>&state);

struct model_parameters{
    float kp;
    float ki;
    float r_esr;
    float r_dab;
    float l_dc;
    float r_dc;
    float v_pri0;
    float v_sec0;
    float f_sw;
    float n_ps;
    float l_dab;
    float c_in;
    float c_out;
    float pi;
    float t_sw;
    float sqrt2;
};


float dab_control(float setpoint, float fb, float *pi_state_next, struct model_parameters p);
void dab_model(float ps, float v_dab_in, float v_cap_out_past, float i_pri_prev, float i_sec_prev, float *i_pri, float *i_sec, struct model_parameters p);
float dab_output(float i_sec, float i_out, float v_cap_out_past, float *v_cap_out, struct model_parameters p);
float input_model(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in, struct model_parameters p);
float input_current(float i_in_past, float v_in, float v_cap_in, float i_pri, struct model_parameters p);

#endif //SD_DAB_HPP
