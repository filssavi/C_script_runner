#ifndef OUTPUT_MANAGER_HPP
#define OUTPUT_MANAGER_HPP

#include <nlohmann/json.hpp>
#include "sciplot/sciplot.hpp"
#include <rapidcsv.h>
#include <utility>

#include "metadata_types.hpp"

class output_manager {
public:
    explicit output_manager(nlohmann::json s);
    void set_outputs(std::vector<std::vector<double>> o) {outputs = std::move(o);}
    void set_timebase(std::vector<double> tb) {timebase = std::move(tb);}
    void output_plot() const;
    void output_data();
    [[nodiscard]] std::vector<model_output> get_outputs() const;
private:


    nlohmann::json specs;
    std::vector<double> timebase;
    std::vector<std::vector<double>> outputs;
    rapidcsv::Document reference_outputs;
};



#endif //OUTPUT_MANAGER_HPP
