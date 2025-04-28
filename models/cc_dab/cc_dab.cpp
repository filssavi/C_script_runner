
#include "cc_dab.hpp"

std::vector<float> cc_dab(const std::vector<float>&inputs, std::vector<float>&state) {

	struct model_parameters params;

	params.kp = 1E-3;
    params.ki = 1;
    params.r_dab = 3.5e-3;
	params.l_dc = 10e-6;
	params.r_dc = 20e-3;
	params.n_ps = 1;
	params.r_esr = 5e-3;
	params.v_0 = 1000;
	params.f_sw = 25e3;
	params.l_dab = 2e-6;
	params.c_in =  1e-3;
	params.c_out =  1e-3;
	params.pi =  3.14159265358979323846;
	params.sqrt2 = 1.41421356237;
    params.t_sw = 1/params.f_sw;


    std::vector<float> outputs(2, 0);

    // OUTPUT

    #define in_v_in inputs[0]
    #define in_en inputs[1]
    #define in_i_out inputs[2]

    #define out_v_out outputs[0]
    #define out_i_in outputs[1]

    #define mem_i_in_past state[0]
    #define mem_i_pri_prev state[1]
    #define mem_v_cap_in_past state[2]
    #define mem_v_cap_out_past state[3]
    #define mem_pi_state state[4]
    #define mem_i_sec_prev state[5]


    float v_cap_in, i_sec, i_pri;

    float v_dab_in = dab_input(mem_i_in_past, mem_i_pri_prev, mem_v_cap_in_past, &v_cap_in, params);

    /// PI

    float ps = dab_control(in_v_in, mem_v_cap_out_past, in_en, &mem_pi_state, params);
    /// PI

    dab_model(ps, v_dab_in, mem_i_pri_prev, mem_v_cap_out_past, mem_i_sec_prev, &i_sec, &i_pri, params);

    out_v_out = dab_output(i_sec, in_i_out, in_en, &mem_v_cap_out_past, params);


    mem_i_in_past = dab_input_current(in_v_in, mem_i_in_past, mem_v_cap_in_past, i_pri, params);

    out_i_in = mem_i_in_past;

    // UPDATE CODE


    mem_i_pri_prev = i_pri;
    mem_i_sec_prev = i_sec;

    mem_v_cap_in_past = v_cap_in;



    return outputs;
}

float satn(float in, float sat){
	if(in < sat) return sat;
	else return in;
};
float satp(float in, float sat){
	if(in > sat) return sat;
  	else return in;
}


float dab_input(float i_in_past, float i_pri_prev, float v_cap_in_past, float *v_cap_in, model_parameters p) {


    double i_cap_in = i_in_past - i_pri_prev;

    *v_cap_in = v_cap_in_past + p.t_sw/p.c_in*i_cap_in;

    float v_dab_in = *v_cap_in + p.r_esr*i_cap_in;

    return v_dab_in;
}


float dab_control(float setpoint, float fb, float en, float *pi_state, model_parameters p){

    float err = (setpoint-fb)*en;

    *pi_state = *pi_state + p.t_sw*err;

	*pi_state = satp(*pi_state, p.pi);
	*pi_state = satn(*pi_state, -p.pi);

    float prop_action = p.kp*err;
    float int_action = p.ki*(*pi_state);

    float ps = prop_action + int_action;

	ps = satp(ps, p.pi);
	ps = satn(ps, -p.pi);


    return ps;
}



void dab_model(float ps, float v_dab_in, float i_pri_prev, float v_cap_out_past, float i_sec_prev, float *i_sec, float *i_pri, model_parameters p){
    float ps_rms = ps/p.sqrt2;

    float ps_factor = ps_rms*(1.0-2.0*ps_rms);


    float k_dab = p.n_ps/(p.f_sw*p.l_dab);

    *i_sec = (v_dab_in-i_pri_prev*p.r_dab)*k_dab*ps_factor;

    *i_pri = (v_cap_out_past-i_sec_prev*p.r_dab)*k_dab*ps_factor;

}


float dab_output(float i_sec, float i_out, float en, float *v_cap_out, model_parameters p){

    float i_cap_out = i_sec - i_out;

    *v_cap_out = *v_cap_out + i_cap_out*p.t_sw/p.c_out;

	bool overrange = (en==0) & (*v_cap_out  <0);
    v_cap_out = overrange ? 0: v_cap_out;

    float v_out = (*v_cap_out + p.r_esr*i_cap_out) - p.r_dc*i_out;
    return v_out;
}


float dab_input_current(float v_in, float i_in_past, float v_cap_in_past, float i_pri, model_parameters p){

    return i_in_past + p.t_sw/p.l_dc*(v_in - p.r_dc*i_in_past - v_cap_in_past -(i_in_past - i_pri)*(p.t_sw/p.c_in + p.r_esr));

}
