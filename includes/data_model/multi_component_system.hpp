

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "data_model/component.hpp"
#include "data_model/modules_cache.hpp"
#include "data_model/system_metadata.hpp"

class multi_component_system {
public:
    explicit multi_component_system(const std::filesystem::path &path, const modules_cache &cache);
    multi_component_system(const multi_component_system &other);

    std::string name;
    std::vector<system_component> components;
    std::vector<system_connection> connections;
    std::vector<model_input> inputs_overloads;
    std::vector<endpoint_descriptor> outputs_overloads;
    uint32_t n_steps;
    float sampling_frequency;
    output_type out_type = plot;
private:
    static endpoint_descriptor parse_endpoint(const std::string &s, const std::string &type, const std::unordered_map<std::string, component> &models);

};



#endif //SYSTEM_HPP
