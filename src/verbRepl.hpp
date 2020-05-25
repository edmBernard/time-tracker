#pragma once

#include <controller.hpp>

#include <cxxsubs.hpp>
#include <replxx.hxx>

#include <fstream>

using Replxx = replxx::Replxx;

Replxx::completions_t hook_completion(std::string const &context, int &contextLen, const Controller &controller) {
  Replxx::completions_t completions;

  std::string prefix{context.substr(contextLen)};

  Replxx::Color c(Replxx::Color::DEFAULT);
  for (auto const &e : controller.taskList(context)) {
      completions.emplace_back(e.c_str(), c);
  }

  return completions;
}

class OptionsRepl : public cxxsubs::IOptions {
public:
  OptionsRepl()
      : cxxsubs::IOptions({"repl"}, "Interactive mode") {
    this->options.add_options()("help", "Print help");
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help() << std::endl;
      exit(0);
    }
  }

  void exec() {
    Replxx rx;
    Controller controller;

    std::string history_file{"./time-tracker-repl-history.txt"};
    rx.history_load(history_file);
    rx.set_completion_callback([&controller](const std::string &context, int &contextLength) { return hook_completion(context, contextLength, controller); });

    std::string prompt{"\x1b[1;32mtimetracker\x1b[0m> "};

    while (true) {
      char const *cinput{nullptr};
      do {
        cinput = rx.input(prompt);
      } while ((cinput == nullptr) && (errno == EAGAIN));

      if (cinput == nullptr) {
        break;
      }
    }

    rx.history_save(history_file);
  };
};
