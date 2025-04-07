

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "data_model/component.hpp"
#include "data_model/modules_cache.hpp"


struct system_connection {
    std::string source;
    std::string destination;
};

struct system_component {
    std::string name;
    std::string type;
    float sampling_frequency;
};


class multi_component_system {
    explicit multi_component_system(nlohmann::json &spec, const modules_cache &cache);
    std::string name;
    std::vector<system_component> components;
    std::vector<system_connection> connections;
    std::vector<model_input> inputs_overloads;
    std::vector<std::string> outputs_overloads;

};



#endif //SYSTEM_HPP
