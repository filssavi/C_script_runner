
#include "sd_dab.hpp"

#include <iostream>
#include <ostream>

std::vector<float> sd_dab(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters) {

    std::vector<float> outputs(2, 0);
    // OUTPUT
    model_parameters p;

    p.kp = parameters[0];
    p.ki = parameters[1];
    p.r_esr = parameters[2];
    p.r_dab = parameters[3];
    p.l_dc = parameters[4];
    p.r_dc = parameters[5];
    p.v_pri0 = parameters[6];
    p.v_sec0 = parameters[7];
    p.f_sw = parameters[8];
    p.n_ps = parameters[9];
    p.l_dab = parameters[10];
    p.c_in = parameters[11];
    p.c_out = parameters[12];
    p.pi = parameters[13];
    p.t_sw = 1/p.f_sw;
    p.sqrt2 = parameters[14];

    #define in_v_in inputs[0]
    #define in_i_out inputs[1]
    #define in_v_ref inputs[2]

    #define out_v_out outputs[0]
    #define out_i_in outputs[1]

    #define mem_pi_state state[0]
    #define mem_i_pri_prev state[1]
    #define mem_i_sec_prev state[2]
    #define mem_v_cap_out_past state[3]
    #define mem_v_cap_in_past state[4]
    #define mem_i_in_past state[5]

    float v_cap_in, i_pri, i_sec,v_cap_out;

    const float v_dab_in = input_model(mem_i_in_past, mem_i_pri_prev,mem_v_cap_in_past, &v_cap_in, p);

    const float ps = dab_control(in_v_ref, mem_v_cap_out_past, &mem_pi_state, p);

    dab_model(ps, v_dab_in, mem_v_cap_out_past, mem_i_pri_prev, mem_i_sec_prev, &i_pri, &i_sec, p);

    out_v_out = dab_output(i_sec, in_i_out,mem_v_cap_out_past, &v_cap_out, p);


    out_i_in = input_current(mem_i_in_past, in_v_in, v_cap_in, i_pri, p);


    // UPDATE CODE

    mem_i_pri_prev = i_pri;
    mem_i_sec_prev = i_sec;

    mem_v_cap_out_past = v_cap_out;
    mem_v_cap_in_past = v_cap_in;

    mem_i_in_past = out_i_in;

    return outputs;
}




float dab_control(float setpoint, float fb, float *pi_state_next, model_parameters p) {

    float err = setpoint - fb;

    *pi_state_next = *pi_state_next + p.t_sw*err;


    if(*pi_state_next > p.pi) *pi_state_next = p.pi;
    if(*pi_state_next < -p.pi) *pi_state_next = -p.pi;

    float prop_action = p.kp*err;
    float int_action = p.ki*(*pi_state_next);

    float ps = prop_action + int_action;

    if(ps > p.pi) ps = p.pi;
    if(ps < -p.pi) ps = -p.pi;
    return ps;

}

void dab_model(float ps, float v_dab_in, float v_cap_out_past, float i_pri_prev, float i_sec_prev, float *i_pri, float *i_sec, model_parameters p){

    float ps_rms = ps/p.sqrt2;

    float ps_factor = ps_rms*(1.0-2.0*ps_rms);


    float k_dab = p.n_ps/(p.f_sw*p.l_dab);

    *i_sec = (v_dab_in-i_pri_prev*p.r_dab)*k_dab*ps_factor;

    *i_pri = (v_cap_out_past-i_sec_prev*p.r_dab)*k_dab*ps_factor;

}


float dab_output(float i_sec, float i_out, float v_cap_out_past, float *v_cap_out, model_parameters p){
    float i_cap_out = i_sec - i_out;

    *v_cap_out = v_cap_out_past + i_cap_out*p.t_sw/p.c_out;

    float v_out = (*v_cap_out + p.r_esr*i_cap_out) - p.r_dc*i_out;

    return v_out;
}

float input_model(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in, model_parameters p){
    float i_cap_in = i_in_past - i_pri_prev;

    *v_cap_in = v_cap_in_past + p.t_sw/p.c_in*i_cap_in;

    float v_dab_in = *v_cap_in + p.r_esr*i_cap_in;
    return v_dab_in;
}


float input_current(float i_in_past, float v_in, float v_cap_in, float i_pri, model_parameters p){
    return i_in_past + p.t_sw/p.l_dc*(v_in - p.r_dc*i_in_past - v_cap_in -(i_in_past - i_pri)*(p.t_sw/p.c_in + p.r_esr));
}