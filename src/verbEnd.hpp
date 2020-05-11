#pragma once

#include <controller.hpp>

#include <cxxsubs.hpp>

#include <fstream>

class OptionsEnd : public cxxsubs::IOptions {
public:
  OptionsEnd()
      : cxxsubs::IOptions({"end"}, "Initialise repository") {
    this->options.add_options()
      ("t,task", "Task name", cxxopts::value<std::string>())
      ("help", "Print help");
    this->options.parse_positional({"task"});
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help({""}) << std::endl;
      exit(0);
    }
    if (!this->parse_result->count("task")) {
      throw cxxopts::OptionParseException("Missing task parameter");
    }
  }

  void exec() {
    auto task = (*this->parse_result)["task"].as<std::string>();
    std::cout << "command : " << this->get_verbs() << ": " << task << std::endl;
    Controller controller;
    controller.add(task, "end");

  };
};
