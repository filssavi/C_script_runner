

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "data_model/component.hpp"
#include "data_model/modules_cache.hpp"


struct endpoint_descriptor {
    std::string component;
    std::string port;
};

struct system_connection {
    endpoint_descriptor source;
    endpoint_descriptor destination;
};

struct system_component {
    std::string name;
    std::string type;
    float sampling_frequency;
};


class multi_component_system {
public:
    explicit multi_component_system(const std::filesystem::path &path);
    std::string name;
    std::vector<system_component> components;
    std::vector<system_connection> connections;
    std::vector<model_input> inputs_overloads;
    std::vector<endpoint_descriptor> outputs_overloads;
    uint32_t n_steps;
private:
    static endpoint_descriptor parse_endpoint(const std::string &s);
};



#endif //SYSTEM_HPP
