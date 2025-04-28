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

#ifndef MODULES_CACHE_HPP
#define MODULES_CACHE_HPP

#include <string>
#include <functional>
#include <openssl/evp.h>

#include <unordered_map>
#include "nlohmann/json.hpp"

#include "utils/settings_store.hpp"
namespace c_script_engine {
    enum module_type {
        component_module,
        system_module
    };

    struct component_metadata {
        std::string name;
        std::string specs_path;
        std::string target_path;
        std::string hash;
        module_type type=component_module;
        bool needs_rebuilding = true;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(component_metadata, name, specs_path, target_path, hash, needs_rebuilding);

    struct system_metadata {
        std::string name;
        std::string specs_path;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(system_metadata, name, specs_path);

    class component;

    class modules_cache {
    public:
        modules_cache();
        void check_cache();

        nlohmann::json dump_lib();
        void index_modules(const std::string& modules_dir);
        void process_module(const std::string& module_path);

        std::optional<std::string> check_cache(const nlohmann::json& module, const std::string &base_path);

        static std::string get_full_path(const std::string &filename, const std::string &base_path);

        std::string hash_file(std::string filename);

        bool contains(const std::string& name) const {return components.contains(name) || systems.contains(name);}

        void clear_rebuild_flag(const std::string& name) {components[name].needs_rebuilding = false;}

        component_metadata get_module_metadata(const std::string& name) const {return components.at(name);}
        component get_component(const std::string &name) const;
        system_metadata get_system_metadata(const std::string& name) const {return systems.at(name);}

        bool is_system(const std::string& name) const {return systems.contains(name);}
        static void clear_cache();
        ~modules_cache();
    private:

        std::unordered_map<std::string, component_metadata> components;
        std::unordered_map<std::string, system_metadata> systems;
    };
}

#endif //MODULES_CACHE_HPP
