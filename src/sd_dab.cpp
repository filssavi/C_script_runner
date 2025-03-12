
#include "sd_dab.hpp"

std::vector<float> sd_dab(const std::vector<float>&inputs, std::vector<float>&state) {

    std::vector<float> outputs(2, 0);
    // OUTPUT

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

    float v_cap_in, pi_state_next, i_pri, i_sec,v_cap_out;

    const float v_dab_in = input_model(mem_i_in_past, mem_i_pri_prev,mem_v_cap_in_past, &v_cap_in);;

    const float ps = dab_control(in_v_ref, mem_v_cap_out_past, mem_pi_state, &pi_state_next);

    dab_model(ps, v_dab_in, mem_v_cap_out_past, mem_i_pri_prev, mem_i_sec_prev, &i_pri, &i_sec);

    out_v_out = dab_output(i_sec, in_i_out,mem_v_cap_out_past, &v_cap_out);


    out_i_in = input_current(mem_i_in_past, in_v_in, v_cap_in, i_pri);


    // UPDATE CODE

    mem_pi_state = pi_state_next;

    mem_i_pri_prev = i_pri;
    mem_i_sec_prev = i_sec;

    mem_v_cap_out_past = v_cap_out;
    mem_v_cap_in_past = v_cap_in;

    mem_i_in_past = out_i_in;

    return outputs;
}




float dab_control(float setpoint, float fb, float pi_state, float *pi_state_next) {

    float err = setpoint - fb;

    *pi_state_next = pi_state + t_sw*err;


    if(*pi_state_next > pi) *pi_state_next = pi;
    if(*pi_state_next < -pi) *pi_state_next = -pi;

    float prop_action = kp*err;
    float int_action = ki*(*pi_state_next);

    float ps = prop_action + int_action;

    if(ps > pi) ps = pi;
    if(ps < -pi) ps = -pi;
    return ps;

}

void dab_model(float ps, float v_dab_in, float v_cap_out_past, float i_pri_prev, float i_sec_prev, float *i_pri, float *i_sec){

    float ps_rms = ps/sqrt2;

    float ps_factor = ps_rms*(1-2*ps_rms);


    float k_dab = n_ps/(f_sw*l_dab);

    *i_sec = (v_dab_in-i_pri_prev*r_dab)*k_dab*ps_factor;

    *i_pri = (v_cap_out_past-i_sec_prev*r_dab)*k_dab*ps_factor;

}


float dab_output(float i_sec, float i_out, float v_cap_out_past, float *v_cap_out){
    float i_cap_out = i_sec - i_out;

    *v_cap_out = v_cap_out_past + i_cap_out*t_sw/c_out;

    float v_out = (*v_cap_out + r_esr*i_cap_out) - r_dc*i_out;

    return v_out;
}

float input_model(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in){
    float i_cap_in = i_in_past - i_pri_prev;

    *v_cap_in = v_cap_in_past + t_sw/c_in*i_cap_in;

    float v_dab_in = *v_cap_in + r_esr*i_cap_in;
    return v_dab_in;
}


float input_current(float i_in_past, float v_in, float v_cap_in, float i_pri){
    return i_in_past + t_sw/l_dc*(v_in - r_dc*i_in_past - v_cap_in -(i_in_past - i_pri)*(t_sw/c_in + r_esr));
}