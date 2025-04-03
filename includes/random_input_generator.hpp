

#ifndef RANDOM_INPUT_GENERATOR_HPP
#define RANDOM_INPUT_GENERATOR_HPP
#include <random>
#include <array>
#include <map>
#include "metadata_types.hpp"


class random_input_generator {
public:

  void set_type(const std::string &name, distribution_type_t t, std::array<float,2> p);
  float get_value(const std::string &name);
private:
  std::map<std::string, distribution_type_t> types;
  std::map<std::string, std::array<float, 2>> parameters;
  std::random_device rd{};
  std::mt19937 gen{rd()};
};



#endif //RANDOM_INPUT_GENERATOR_HPP
