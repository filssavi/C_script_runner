

#include "data_model/multi_component_system.hpp"
namespace c_script_engine {
    multi_component_system::multi_component_system(const std::filesystem::path &path, const modules_cache &cache) {


        nlohmann::json spec;
        std::ifstream spec_stream(path);
        spec_stream >> spec;

        name = spec["name"];
        n_steps = spec["run_length"];

        std::unordered_map<std::string, component> models;

        for(auto &model: spec["models"]) {
            system_component c;
            c.name = model["name"];
            c. type = model["type"];
            c.sampling_frequency = model["sampling_frequency"];
            sampling_frequency = c.sampling_frequency; // todo: make this simulator multi_step capable
            models[c.name] = cache.get_component(c.type);
            components.push_back(c);
        }
        for(auto &conn: spec["connections"]) {
            system_connection c;
            c.source = parse_endpoint(conn["source"], "out", models);
            c.destination = parse_endpoint(conn["destination"],"in", models);
            connections.push_back(c);
        }
        for(auto &in: spec["inputs"]) {
            model_input i(in, {}, n_steps);
            inputs_overloads.push_back(i);
        }
        for(auto &out: spec["outputs"]) {
            outputs_overloads.push_back(parse_endpoint(out,"out", models));
        }
    }

    multi_component_system::multi_component_system(const multi_component_system &other) {

        components = other.components;
        name = other.name;
        n_steps = other.n_steps;
        inputs_overloads = other.inputs_overloads;
        outputs_overloads = other.outputs_overloads;
        connections = other.connections;
        sampling_frequency = other.sampling_frequency;
        out_type = other.out_type;
    }

    endpoint_descriptor multi_component_system::parse_endpoint(const std::string &s, const std::string &type,
        const std::unordered_map<std::string, component> &models) {

        auto split_point = s.find('.');
        auto comp = s.substr(0, split_point);
        auto port = s.substr(split_point + 1);
        uint32_t order = 99;
        if(type =="in") {
            for(auto &in: models.at(comp).inputs) {
                if(in.name == port) {
                    order = in.input_index;
                }
            }
        } else {
            for(auto &in: models.at(comp).outputs) {
                if(in.name == port) {
                    order = in.output_index;
                }
            }
        }

        return {comp,port , order};
    }


    std::vector<system_component> components;
    std::vector<system_connection> connections;
    std::vector<model_input> inputs_overloads;
    std::vector<std::string> outputs_overloads;
}