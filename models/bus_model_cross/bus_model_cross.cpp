 #include "bus_model_cross.hpp"

 std::vector<float> bus_model_cross(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters) {


    model_parameters params;
    params.f_sw = 1/parameters[0];
    params.r_in = parameters[1];
    params.v_0 = parameters[2];
    params.p_l = parameters[3];

    std::vector<float> outputs(3, 0);

    // OUTPUT

    #define in_v_in inputs[0]
    #define in_i_out inputs[1]
    #define in_v_cross inputs[2]
    #define in_fault inputs[2]

    #define out_v_out outputs[0]
    #define out_i_in outputs[1]
    #define out_i_cross outputs[2]

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