

#include "data_model/multi_component_system.hpp"

multi_component_system::multi_component_system(const std::filesystem::path &path) {


    nlohmann::json spec;
    std::ifstream spec_stream(path);
    spec_stream >> spec;

    name = spec["name"];
    n_steps = spec["run_length"];

    for(auto &model: spec["models"]) {
        system_component c;
        c.name = model["name"];
        c. type = model["type"];
        c.sampling_frequency = model["sampling_frequency"];
        components.push_back(c);
    }
    for(auto &conn: spec["connections"]) {
        system_connection c;
        auto point = std::string().find('.');
        c.source = parse_endpoint(conn["source"]);
        c.destination = parse_endpoint(conn["destination"]);
        connections.push_back(c);
    }
    for(auto &in: spec["inputs"]) {
        model_input i(in, {}, n_steps);
        inputs_overloads.push_back(i);
    }
    for(auto &out: spec["outputs"]) {
        outputs_overloads.push_back(parse_endpoint(out));
    }
}

endpoint_descriptor multi_component_system::parse_endpoint(const std::string &s) {
    auto split_point = s.find('.');
    return {s.substr(0, split_point), s.substr(split_point + 1)};
}

std::vector<system_component> components;
std::vector<system_connection> connections;
std::vector<model_input> inputs_overloads;
std::vector<std::string> outputs_overloads;
