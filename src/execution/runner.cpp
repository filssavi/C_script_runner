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

#include "execution/runner.hpp"

void run(const std::variant<component, multi_component_system> &model) {

    if(std::holds_alternative<component>(model)) {
        component_runner c(std::get<component>(model));
        c.run_emulation();
        c.process_output();
    } else {
        system_runner s(std::get<multi_component_system>(model));
        s.run_emulation();
        s.process_output();
    }
}
