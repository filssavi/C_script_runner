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

struct module_metadata {
    std::string name;
    std::string specs_path;
    std::string target_path;
    std::string hash;
    bool needs_rebuilding = true;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(module_metadata, name, specs_path, target_path, hash);

class modules_cache {
public:
    modules_cache();
    void index_modules(const std::string& modules_dir);
    void process_module(const std::string& module_path);

    std::optional<std::string> check_cache(const nlohmann::json& module, const std::string &base_path);

    static std::string get_full_path(const std::string &filename, const std::string &base_path);

    std::string hash_file(std::string filename);

    bool contains(const std::string& name) {return modules.contains(name);};
    module_metadata get_module(const std::string& name) {return modules[name];};

    ~modules_cache();
private:

    std::unordered_map<std::string, module_metadata> modules;
};



#endif //MODULES_CACHE_HPP
