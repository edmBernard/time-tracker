#pragma once

#include <controller.hpp>

#include <cxxsubs.hpp>

#include <fstream>


class OptionsTaskCompletion : public cxxsubs::IOptions {
public:
  OptionsTaskCompletion()
      : cxxsubs::IOptions({"task_completion"}, "Task completion command") {
    this->options.add_options()
      ("help", "Print help")
      ("s,search", "String to search in task name", cxxopts::value<std::string>()->default_value(""), "SEARCH");
    this->options.parse_positional({"search"});
    this->options.positional_help("[<search>]");
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help() << std::endl;
      exit(0);
    }
  }

  void exec() {
    auto task = (*this->parse_result)["search"].as<std::string>();
    Controller controller;
    auto tasks = controller.taskList(task);

    for (auto i : tasks) {
      std::cout << i << std::endl;
    }
  };
};
