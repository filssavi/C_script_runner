

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

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <variant>


#include "execution/component_runner.hpp"
#include "execution/system_runner.hpp"

#include "data_model/component.hpp"
#include "data_model/modules_cache.hpp"
#include"data_model/multi_component_system.hpp"
namespace c_script_engine {
    void run(const std::variant<component, multi_component_system>&model, modules_cache &cache);
}

#endif //RUNNER_HPP
