#pragma once

#include <controller.hpp>

#include <fmt/format.h>
#include <cxxsubs.hpp>

#include <fstream>


class OptionsList : public cxxsubs::IOptions {
public:
  OptionsList()
      : cxxsubs::IOptions({"list"}, "list all saved data") {
    this->options.add_options()
      ("help", "Print help")
      ("l,limit", "Nummber of line to show", cxxopts::value<int>()->default_value("5"))
      ;
    this->options.parse_positional({"limit"});
    this->options.positional_help("[limit]");
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help() << std::endl;
      exit(0);
    }
  }

  void exec() {
    auto limit = (*this->parse_result)["limit"].as<int>();
    Controller controller;
    auto tasks = controller.list(limit);

    for (auto & i : tasks) {
      fmt::print("{: <30} {: <10} {}\n", std::get<0>(i), std::get<1>(i), std::get<2>(i));
    }
  };
};
