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

    float v_cap;
    std::cout<< "in_fault = " << in_fault<< std::endl;
    if(in_fault) {

        v_cap = mem_v_cap0 - (in_i_out + params.i_l)*params.t_sw/params.c;

        out_v_out = v_cap- params.r_esr*(in_i_out+ params.i_l);
        if(out_v_out<0) out_v_out = 0;
        out_i_in = 0;

    } else {

        float v_out_num = (params.i_l + in_i_out)*(params.t_sw/params.c - params.r_esr)*params.r_in - in_v_in*(params.t_sw/params.c - params.r_esr) + params.r_in*mem_v_cap0;

        float v_out_den = params.t_sw/params.c - params.r_esr - params.r_in;

        out_v_out = - v_out_num/v_out_den;

        float i_cap = (out_v_out - mem_v_cap0)/(params.t_sw/params.c - params.r_esr);

        v_cap = out_v_out + params.r_esr*i_cap;

        out_i_in = i_cap + params.i_l + in_i_out;
    }

    return outputs;
}

