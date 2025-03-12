#ifndef INPUTS_MANAGER_HPP
#define INPUTS_MANAGER_HPP

#include <rapidcsv.h>
#include <nlohmann/json.hpp>
#include <vector>

#include "metadata_types.hpp"


class inputs_manager {
public:
    inputs_manager(const nlohmann::json &config);
    std::vector<model_input> get_inputs();
private:
    nlohmann::json specs;
};



#endif //INPUTS_MANAGER_HPP
