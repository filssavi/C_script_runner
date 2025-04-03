

#include "data_model/system.hpp"

multi_component_system::multi_component_system(nlohmann::json &spec) {
    name = spec["name"];
    for(auto &model: spec["models"]) {

    }
    for(auto &conn: spec["connections"]) {

    }
}
