//
// Created by filssavi on 3/12/25.
//

#include "../includes/inputs_manager.hpp"

inputs_manager::inputs_manager(const nlohmann::json &config) {
    specs = config;
}

std::vector<model_input> inputs_manager::get_inputs() {

    const auto inputs_file = std::filesystem::canonical(specs["inputs"]["series_file"]);
    const auto doc = rapidcsv::Document(inputs_file, rapidcsv::LabelParams(0, -1));

    std::vector<model_input> ret_val;
    for (auto &in:specs["inputs"]["specs"]) {
        model_input i;
        i.name = in["name"];
        i.input_index = in["model_order"];

        if (in["type"] == "constant") {
            i.type = constant_input;
            i.const_value = in["value"];
        } else {
            i.type = series_input;
            const uint8_t idx = in["series_order"];
            i.series_values = doc.GetColumn<float>(idx);
        }

        ret_val.push_back(i);
    }
    return ret_val;
}
