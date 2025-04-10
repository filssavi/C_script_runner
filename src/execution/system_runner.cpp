

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

#include "execution/system_runner.hpp"

system_runner::system_runner(const multi_component_system &sys, modules_cache &cache) : system(sys){

    for(auto &c:sys.components) {
        if(!cache.contains(c.type)) {
            std::cout << "Error: component "<< c.type << "not found" <<std::endl;
        }
        auto component_metadata = cache.get_module(c.type);
        components[c.name] = component_metadata;
        if (component_metadata.needs_rebuilding) {
            builder::build_module(component_metadata);
            cache.clear_rebuild_flag(component_metadata.name);
        }

        auto path =  std::filesystem::path(component_metadata.target_path);
        auto base_path = path.parent_path().string();
        auto exec_path = base_path + "/lib" + path.filename().replace_extension().string() + ".so";
        targets[c.name] = load_dll(exec_path, component_metadata.name);

        component comp(component_metadata.specs_path);
        for(auto s:comp.states){
            states[c.name].push_back(s);
        }

    }
    system = sys;
}

void system_runner::run_emulation() {
    int i  = 0;

    //TODO: SETUP STATES
    // LOOP
        //TODO: INPUTS PHASE
        //TODO: EMULATION PHASE
        //TODO: OUTPUT PHASE
}

void system_runner::process_output() {

}

target_cscript_t system_runner::load_dll(const std::string &path, const std::string &module_name) {
    void* handle = dlopen(path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot open library: " << dlerror() << '\n';
        exit(1);
    }
    dlerror();  // Clear any existing error

    target_cscript_t t = (target_cscript_t) dlsym(handle, module_name.c_str());

    if (const char* dlsym_error = dlerror()) {
        std::cerr << "Cannot load symbol: " << dlsym_error << '\n';
        dlclose(handle);
        exit(1);
    }
    return t;
}
