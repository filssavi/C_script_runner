#include <string>
#include <filesystem>
#include <CLI/CLI.hpp>
#include <nlohmann/json.hpp>

#include "inputs_manager.hpp"
#include "output_manager.hpp"
#include "runner.hpp"

std::string get_full_path(const std::string& filename, const std::string &base_path) {
    std::string path;
    if(!path.starts_with(".") || path.starts_with("/")) {
        path =base_path + "/" + filename;
    } else {
        path = std::filesystem::canonical(filename);
    }
    return path;
}

std::tuple<std::string, std::string, std::string> validate_specs(const nlohmann::json &specs, const std::string &base_path) {

    if (!specs["model"].contains("target_name")) {
        std::cout<<"Target name not specified"<<std::endl;
        exit(1);
    }
    if (!specs["model"].contains("target_path")) {
        std::cout<<"Target path not specified"<<std::endl;
        exit(1);
    }

    const auto target_path = get_full_path(specs["model"]["target_path"], base_path);
    if (!std::filesystem::exists(target_path)) {
        std::cerr << "Target SO file does not exist" << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(target_path)) {
        std::cerr << "Target SO path points to a directory, not a file" << std::endl;
        exit(1);
    }

    const auto reference_path = get_full_path(specs["reference_outputs"], base_path);
    if (!std::filesystem::exists(reference_path)) {
        std::cerr << "Reference output file does not exist" << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(reference_path)) {
        std::cerr << "Reference output path points to a directory, not a file" << std::endl;
        exit(1);
    }


    const auto inputs_path = get_full_path(specs["inputs"]["series_file"], base_path);
    if (!std::filesystem::exists(inputs_path)) {
        std::cerr << "Inputs file does not exist" << std::endl;
        exit(1);
    }
    if (std::filesystem::is_directory(inputs_path)) {
        std::cerr << "Inputs path points to a directory, not a file" << std::endl;
        exit(1);
    }

    return {target_path, reference_path, inputs_path};
}

std::vector<float> get_initial_state(const nlohmann::json& states) {
    std::vector<float> initial_states(states.size());
    for (const auto &s : states) {
        initial_states[s["order"]] = s["initial_value"];
    }
    return initial_states;
}

void compile(const std::filesystem::path &file) {
    const std::string file_path ="lib" +file.stem().string() + std::string(".so");
    if (std::filesystem::exists(file_path)) {
        std::filesystem::remove_all(file_path);
    }
    std::vector<std::string> compile_command_args = {
        "g++",
        "-fPIC",
        "-shared",
        file.string(),
        "-o",
        file_path
    };
    std::string command;
    for (auto &p:compile_command_args) command += p + " ";
    std::system(command.c_str());
}

void run(std::istream &spec_stream,const std::string &base_path) {

    nlohmann::json specs;
    spec_stream >> specs;

    auto [target_path, reference_path, inputs_path] = validate_specs(specs, base_path);

    std::string name = specs["model"]["target_name"];
    auto path = target_path;

    inputs_manager in_mgr(specs, inputs_path);
    output_manager out_mgr(specs, reference_path);

    runner r;
    r.set_target(name, path);
    r.add_inputs(in_mgr.get_inputs());
    r.add_outputs(out_mgr.get_outputs());

    r.initialize_states(get_initial_state(specs["states"]));
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
}
int main(int argc, char **argv) {

    CLI::App app{"General purpose runner for C-script derived functions"};

    std::string spec_file;
    bool build = false;
    app.add_option("spec", spec_file, "specifications file");
    app.add_flag("--compile" ,build, "File to compile to a dynamically loadable library");
    CLI11_PARSE(app, argc, argv);

    auto parent = absolute(std::filesystem::path(spec_file).parent_path());

    if (build) {
        std::string current_dir = std::filesystem::current_path().string();
        current_path(parent);
        compile(std::filesystem::path(spec_file).filename());
        std::filesystem::current_path(current_dir);
    } else {
        std::ifstream spec_stream(spec_file);
        run(spec_stream, parent);
    }
    return 0;
}
