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

#include "utils/csv_interface.hpp"

#include <iostream>


namespace c_script_engine {
    std::unordered_map<std::string, std::vector<double>> csv_interface::parse_file(std::string filename) {
        std::ifstream file(filename);
        std::vector<std::string> columns;
        std::vector<std::vector<double>> raw_data;

        std::string header;
        std::getline(file, header, '\n');
        if(std::isdigit(header[0])) {
            std::cerr << "Detected CSV file (" << filename<< ") without header" << std::endl;
            exit(0);
        }


        std::istringstream iss(header);
        for (std::string val_s; std::getline(iss, val_s, ',');) {
            columns.push_back(val_s);
            raw_data.emplace_back();
        }


        for (std::string line; std::getline(file, line, '\n');) {
            iss = std::istringstream(line);
            int idx = 0;
            for (std::string val_s; std::getline(iss , val_s, ',');) {
                raw_data[idx].push_back(std::stof(val_s));
                idx++;
            }

        }

        std::unordered_map<std::string, std::vector<double>> ret_val;
        for(int i = 0; i < columns.size(); ++i) {
            ret_val[columns[i]] = raw_data[i];
        }
        return ret_val;
    }

    void csv_interface::write_file(std::string filename, std::vector<std::pair<std::string, std::vector<double>>> data) {
        std::ofstream file(filename);
        std::stringstream ss;
        std::vector<std::vector<double>> raw_data;
        for(auto &[key, value] : data) {
            ss << key << ",";
            raw_data.push_back(value);
        }
        auto header = ss.str();
        header.pop_back();
        file << header << std::endl;

        for(int i = 0; i < raw_data[0].size(); ++i) {
            std::stringstream line;
            for(int j = 0; j < raw_data.size(); ++j) {
                line << raw_data[j][i];
                if(j != raw_data.size() - 1) {
                    line << ",";
                } else {
                    line << std::endl;
                }
            }
            auto l = line.str();
            file << l;
        }
    }
}

