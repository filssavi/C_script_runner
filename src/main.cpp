#include <string>
#include <filesystem>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include "inputs_manager.hpp"
#include "output_manager.hpp"
#include "runner.hpp"



void validate_specs(const nlohmann::json &specs) {

    if (!specs["model"].contains("target_name")) {
        std::cout<<"Target name not specified"<<std::endl;
        exit(1);
    }
    if (!specs["model"].contains("target_path")) {
        std::cout<<"Target path not specified"<<std::endl;
        exit(1);
    }


    if (!std::filesystem::exists(specs["model"]["target_path"])) {
        std::cerr << "Target SO file does not exist" << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(specs["model"]["target_path"])) {
        std::cerr << "Target SO path points to a directory, not a file" << std::endl;
        exit(1);
    }

    if (!std::filesystem::exists(specs["reference_outputs"])) {
        std::cerr << "Reference output file does not exist" << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(specs["reference_outputs"])) {
        std::cerr << "Reference output path points to a directory, not a file" << std::endl;
        exit(1);
    }

}

int main(int argc, char **argv) {

    CLI::App app{"General purpose runner for C-script derived functions"};

    std::string spec_file;
    app.add_option("spec", spec_file, "JSON specifications file")->check(CLI::ExistingFile);
    CLI11_PARSE(app, argc, argv);


    nlohmann::json specs;
    std::ifstream spec_stream(spec_file);
    spec_stream >> specs;

    validate_specs(specs);

    std::string name = specs["model"]["target_name"];
    auto path = std::filesystem::canonical(specs["model"]["target_path"]);

    inputs_manager in_mgr(specs);
    output_manager out_mgr(specs);

    runner r;
    r.set_target(name, path);
    r.add_inputs(in_mgr.get_inputs());
    r.add_outputs(out_mgr.get_outputs());

    r.set_n_states(specs["model"]["states"]);
    r.set_f_sample(specs["model"]["sampling_frequency"]);
    r.set_n_steps(specs["run_length"]);
    r.run_emulation();

    out_mgr.set_timebase(r.get_timebase());
    out_mgr.set_outputs(r.get_outputs());

    if (specs["outputs"]["type"]=="plot") {
        out_mgr.output_plot();
    } else if (specs["outputs"]["type"]=="csv") {
        out_mgr.output_data();
    }

    return 0;
}