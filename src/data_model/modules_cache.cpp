

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

#include "data_model/modules_cache.hpp"

modules_cache::modules_cache() {
    auto cache_path = settings_store::instance().get_path("modules_cache");
    auto modules_path = settings_store::instance().get_path("modules");

    if(exists(cache_path)) {
        modules = nlohmann::json::parse(std::ifstream(cache_path));
    }
    index_modules(modules_path);
}

void modules_cache::index_modules(const std::string& modules_dir) {
    for(auto &m:std::filesystem::directory_iterator(modules_dir)) {
        if(m.is_directory()){
            for(auto &f:std::filesystem::directory_iterator(m.path())) {
                if(f.is_regular_file()) {
                    if(f.path().extension() == ".json") {
                        process_module(f.path());
                    }
                }
            }
        }
    }
}

void modules_cache::process_module(const std::string &module_path) {

    auto module_spec = nlohmann::json::parse(std::ifstream(module_path));
    if(module_spec["type"] == "component") {
        module_metadata data;
        data.specs_path = module_path;
        data.name = module_spec["model"]["target_name"];
        auto path_base =std::filesystem::path(module_path).parent_path();
        data.target_path = get_full_path(module_spec["model"]["target_path"], path_base);
        if(auto hash = check_cache(module_spec, path_base)) {
            data.needs_rebuilding = true;
            data.hash = hash.value();
        } else {
            data.needs_rebuilding = modules[module_spec["model"]["target_name"]].needs_rebuilding;
            data.hash = modules[module_spec["model"]["target_name"]].hash;
        }
        modules[data.name] = data;
    }
}

std::optional<std::string> modules_cache::check_cache(const nlohmann::json &module, const std::string &base_path) {

    std::string target_path = get_full_path(module["model"]["target_path"], base_path);
    auto name = std::filesystem::path(module["model"]["target_path"]).stem().string();
    auto on_disk_hash = hash_file(target_path);

    if(!std::filesystem::exists("lib" + name + ".so")) {
        return on_disk_hash;
    }
    if(modules[module["model"]["target_name"]].hash != on_disk_hash) return on_disk_hash;
    return {};
}

std::string modules_cache::get_full_path(const std::string &filename, const std::string &base_path) {
    std::string path;
    if(!path.starts_with(".") || path.starts_with("/")) {
        path =base_path + "/" + filename;
    } else {
        path = std::filesystem::canonical(filename);
    }
    return path;
}

std::string modules_cache::hash_file(std::string target_path) {
    std::ifstream ifs(target_path);
    std::string on_disk_content((std::istreambuf_iterator<char>(ifs)),
                         std::istreambuf_iterator<char>());

    EVP_MD_CTX* context = EVP_MD_CTX_new();

    if(context != nullptr){
        if(EVP_DigestInit_ex(context, EVP_sha256(), nullptr)) {
            if(EVP_DigestUpdate(context, on_disk_content.c_str(), on_disk_content.length())) {
                unsigned char hash[EVP_MAX_MD_SIZE];
                unsigned int lengthOfHash = 0;

                if(EVP_DigestFinal_ex(context, hash, &lengthOfHash)) {
                    std::stringstream ss;
                    for(unsigned int i = 0; i < lengthOfHash; ++i) {
                        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
                        if(i<lengthOfHash-1){
                            ss<<":";
                        }
                    }
                    EVP_MD_CTX_free(context);
                    return ss.str();
                }
            }
        }
    }


    // If I get here there must have been some error with the hash calculation, throw an exception
    throw std::runtime_error("ERROR: could not calculate file hash");
}

modules_cache::~modules_cache() {

    auto cache_path = settings_store::instance().get_path("modules_cache");
    std::ofstream ofs(cache_path);
    ofs << nlohmann::json(modules);
}
