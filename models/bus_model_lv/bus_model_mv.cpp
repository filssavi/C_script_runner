 #include "bus_model_mv.hpp"

 std::vector<float> bus_model_mv(const std::vector<float>&inputs, std::vector<float>&state) {


    model_parameters params;
    params.t_sw = 1/25e3;
    params.c = 1e-3;
    params.r_in = 1e-3;
    params.r_esr = 1e-6;
    params.v_0 = 250;
    params.i_l = 20e3/params.v_0;

    std::vector<float> outputs(2, 0);

    // OUTPUT

    #define in_v_in inputs[0]
    #define in_i_out inputs[1]
    #define in_fault inputs[2]

    #define out_v_out outputs[0]
    #define out_i_in outputs[1]

    #define mem_v_cap0 state[0]


    bus(in_i_out, in_v_in, in_fault, out_v_out, out_i_in, params);

    return outputs;
}

void bus(float i_out, float v_in, int fault, float &v_out, float &i_in, model_parameters &params) {
    if(fault) {

        v_out = 0;
        i_in = 0;

    } else {

        i_in = params.i_l + i_out;

        v_out = v_in -  params.r_in*(i_in + i_out);

    }

}