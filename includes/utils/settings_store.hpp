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

#ifndef SETTINGS_STORE_HPP
#define SETTINGS_STORE_HPP

#include <toml++/toml.hpp>
#include <iostream>
#include <filesystem>
#include <unordered_map>

class settings_store {
    public:

    static settings_store& instance() {
        static settings_store instance; // Guaranteed to be thread-safe in C++11 and later
        return instance;
    }

    // Delete copy constructor and assignment operator to prevent creating multiple instances
    settings_store(const settings_store&) = delete;
    settings_store& operator=(const settings_store&) = delete;


    void create_defaults();
    std::filesystem::path get_path(const std::string & name);

private:
    settings_store();
    ~settings_store();


    std::unordered_map<std::string, std::string> paths_default={
        {"modules", "modules"},
        {"modules_cache", "modules_cache.json"}
    };
    toml::table config;
    std::string settings_file;
    std::string settings_path;
};



#endif //SETTINGS_STORE_HPP
