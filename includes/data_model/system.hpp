

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "data_model/component.hpp"

struct component_interconnection {
    port_metadata source;
    port_metadata destination;
};

class multi_component_system {
    explicit multi_component_system(nlohmann::json &spec);
    std::string name;
    std::vector<component> components;
    std::vector<component_interconnection> connections;
    std::vector<model_input> inputs_overloads;
    std::vector<model_output> outputs_overloads;

};



#endif //SYSTEM_HPP
