 #include "bus_model_cross.hpp"

#include <iostream>
#include <ostream>

 std::vector<float> bus_model_cross_source(const std::vector<float> &inputs, std::vector<float> &state,
                                           const std::vector<float> &parameters) {
     return bus_model_cross(inputs, state, parameters);
 }

 std::vector<float> bus_model_cross_sink(const std::vector<float> &inputs, std::vector<float> &state,
                                         const std::vector<float> &parameters) {
     return bus_model_cross(inputs, state, parameters);
 }

std::vector<float> bus_model_cross(const std::vector<float>&inputs, std::vector<float>&state, const std::vector<float> &parameters) {


    model_parameters params;
    params.v_0 = state[0];
    params.p_l = state[1];
    params.mode = parameters[0];

    std::vector<float> outputs(4, 0);

    // OUTPUT

    #define in_v_in inputs[0]
    #define in_i_out inputs[1]
    #define in_fault inputs[2]
    #define in_v_cross inputs[3]
    #define in_i_cross inputs[4]


    #define out_v_out outputs[0]
    #define out_i_in outputs[1]
    #define out_i_cross outputs[2]
    #define out_v_cross outputs[3]

    if(params.mode  == 0) {
        // SOURCE MODE
        float i_cross = in_i_cross;
        bus(in_i_out, in_v_in, in_fault, out_v_out, out_i_in, out_v_cross, i_cross, params);
    } else {
        //SINK MODE
        float v_cross = in_v_cross;
        bus(in_i_out, in_v_in, in_fault, out_v_out, out_i_in, v_cross, out_i_cross, params);
    }

    return outputs;
}

void bus(float i_out, float v_in, int fault, float &v_out, float &i_in, float &v_cross, float &i_cross, const model_parameters &params) {
    const float i_l = params.p_l/params.v_0;

     if(params.mode == 0){
         i_in = i_l + i_out  + i_cross;
         v_cross = v_in;
         v_out = v_in;

     }else {

         if(fault) {
             i_cross = i_out + i_l;
             v_out = v_cross;
             i_in = 0;

         } else {

             i_in = i_l + i_out;
             v_out = v_in;

             i_cross = 0;
         }

     }
}