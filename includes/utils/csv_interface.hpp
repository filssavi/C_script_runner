//  Copyright 2025 Filippo Savi <filssavi@gmail.com>
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef CSV_INTERFACE_HPP
#define CSV_INTERFACE_HPP

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class csv_interface {

  public:
    static std::unordered_map<std::string, std::vector<double>> parse_file(std::string filename);
    static void write_file(std::string filename, std::unordered_map<std::string, std::vector<float>> data);
};



#endif //CSV_INTERFACE_HPP
