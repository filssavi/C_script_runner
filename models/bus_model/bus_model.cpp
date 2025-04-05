 #include "bus_model.hpp"
#include <iostream>

std::vector<float> bus_model(const std::vector<float>&inputs, std::vector<float>&state) {


    model_parameters params;
    params.t_sw = 1/25e3;
    params.c = 1e-3;
    params.r_in = 1e-3;
    params.r_esr = 1e-6;
    params.v_0 = 1000;
    params.i_l = 400e3/1000;

    std::vector<float> outputs(2, 0);

    // OUTPUT

    #define in_v_in inputs[0]
    #define in_i_out inputs[1]
    #define in_fault inputs[2]

    #define out_v_out outputs[0]
    #define out_i_in outputs[1]

    #define mem_v_cap0 state[0]

    float v_cap = mem_v_cap0;

    bus(in_i_out, in_v_in, in_fault, out_v_out, out_i_in, v_cap, params);
    mem_v_cap0 = v_cap;
    return outputs;
}

void bus(float i_out, float v_in, int fault, float &v_out, float &i_in, float &v_cap, struct model_parameters &params) {
    if(fault) {

        v_cap = v_cap - (i_out + params.i_l)*params.t_sw/params.c;

        v_out = v_cap- params.r_esr*(i_out+ params.i_l);
        if(v_out<0) v_out = 0;
        i_in = 0;

    } else {

        float v_out_num = (params.i_l + i_out)*(params.t_sw/params.c - params.r_esr)*params.r_in - v_in*(params.t_sw/params.c - params.r_esr) + params.r_in*v_cap;

        float v_out_den = params.t_sw/params.c - params.r_esr - params.r_in;

        v_out = - v_out_num/v_out_den;

        float i_cap = (v_out - v_cap)/(params.t_sw/params.c - params.r_esr);

        v_cap = v_out + params.r_esr*i_cap;

        i_in = i_cap + params.i_l + i_out;
    }

}