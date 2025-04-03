

#ifndef RANDOM_INPUT_GENERATOR_HPP
#define RANDOM_INPUT_GENERATOR_HPP
#include <random>


class random_input_generator {
public:
  enum distribution_type {
    normal,
    uniform
  };
  void set_type(distribution_type t, std::vector<float> p);
  float get_value();
private:
  distribution_type type = uniform;
  std::vector<float> parameters;
  std::random_device rd{};
  std::mt19937 gen{rd()};
};



#endif //RANDOM_INPUT_GENERATOR_HPP
