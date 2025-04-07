

#include "data_model/system.hpp"

multi_component_system::multi_component_system(nlohmann::json &spec, const modules_cache &cache) {
    name = spec["name"];
    for(auto &model: spec["models"]) {
        auto module = cache.get_module(model["name"]);
        auto target = module.target_path;
        auto i = 0;
    }
    for(auto &conn: spec["connections"]) {

    }
}
