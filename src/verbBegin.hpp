#pragma once

#include <controller.hpp>

#include <cxxsubs.hpp>

#include <fstream>

class OptionsBegin : public cxxsubs::IOptions {
public:
  OptionsBegin()
      : cxxsubs::IOptions({"add"}, "Initialise repository") {
    this->options.add_options()
      ("m, module", "module name", cxxopts::value<std::string>()->default_value("my_module"))
      ("help", "Print help");
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }
  }

  void exec() {
    std::cout << "command : " << this->get_verbs() << std::endl;
    std::cout << "module : " << (*this->parse_result)["module"].as<std::string>() << std::endl;

    Controller controller;
    controller.add("task");

  };
};
