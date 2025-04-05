// Copyright 2025 Filippo Savi <filssavi@gmail.com>
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "utils/settings_store.hpp"



settings_store::settings_store() {
    if (const char* env_p = std::getenv("SETTINGS_PATH")) {
        settings_file = env_p;
    } else {
        settings_file = std::getenv("HOME");
        settings_file += "/.config/c_script_runner/settings.toml";

    }
    settings_path = std::filesystem::path(settings_file).parent_path();


    if(!std::filesystem::exists(settings_file)) {
        std::cout<< "The settings file '" << settings_file << "' does not exist." << std::endl;
        std::cout<< "should I create one?" << std::endl;
        std::string response;
        std::cin >> response;
        if(response == "y" || response == "yes" || response == "Y") create_defaults();
        else return;
    }
    config = toml::parse_file(settings_file);
}

void settings_store::create_defaults() {
    const auto dir = std::filesystem::path(settings_file).parent_path();
    create_directories(dir);
    std::ofstream file(settings_file);
    std::string home =  std::getenv("HOME");
    auto modules_path = settings_path + "/modules";

    if(!std::filesystem::exists(modules_path)) std::filesystem::create_directory(modules_path);
    toml::table dfl_settings{
            {"paths",toml::table{}}
    };

    for(auto  &[name, value]: paths_default) {
        dfl_settings["paths"].as_table()->insert_or_assign(name, toml::value{settings_path + "/" + value});
    }
    file << dfl_settings;
    file.close();
}

std::filesystem::path settings_store::get_path(const std::string &name) {
    auto paths = config["paths"];
    std::optional<std::string>  res = paths[name].value<std::string>();
    if(!res.has_value()) {
        std::cout<< "The path '" << name << "' is not in the settings store." << std::endl;
        exit(0);
    }
    return {res.value()};
}



settings_store::~settings_store() {
    std::fstream file(settings_file);
    file << config;
    file.close();
}
