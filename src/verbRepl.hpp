#pragma once

#include <controller.hpp>

#include <cxxsubs.hpp>
#include <replxx.hxx>
#include <spdlog/spdlog.h>

#include <fstream>

using Replxx = replxx::Replxx;

int customSplit(const std::string & s) {
	const std::string wb = " \t\n\r\v\f-=+*&^%$#@!,./?<>;:`~'\"[]{}()\\|";
	int cl = 0;
  for (int i = s.length() - 1; i >= 0; --i) {
    if (wb.find(s[i], 0) == 0) {
      // break on whitespace
      return cl;
    }
    ++cl;
	}
	return cl;
}

Replxx::completions_t hook_completion(std::string const &context, int &contextLen, const Controller &controller) {
  Replxx::completions_t completions;

  // add support for special character in context by default the context is split on first special character found
  contextLen = customSplit(context);

  Replxx::Color c(Replxx::Color::DEFAULT);

  // add task list completion if begin or end
  if (context.rfind("begin ", 0) != std::string::npos || context.rfind("end ", 0) != std::string::npos) {
    for (auto const &e : controller.taskList(context.substr(context.length() - contextLen))) {
      completions.emplace_back(e.c_str(), c);
    }
  }

  // add completion for command
  for (auto const &e : std::vector<std::string>{".quit", ".exit", "begin", "end", "list"}) {
    if (e.rfind(context, 0) == 0) {
      Replxx::Color c(Replxx::Color::DEFAULT);
      completions.emplace_back((e + " ").c_str(), c);
    }
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

    std::string history_file{"./timetracker-history.txt"};
    rx.history_load(history_file);
    rx.set_completion_callback( std::bind( &hook_completion, std::placeholders::_1, std::placeholders::_2, std::ref(controller)));

    std::string prompt{"\x1b[1;32mtimetracker\x1b[0m> "};

    while (true) {
      char const *cinput{nullptr};
      do {
        cinput = rx.input(prompt);
      } while ((cinput == nullptr) && (errno == EAGAIN));

      if (cinput == nullptr) {
        break;
      }

      std::string input{cinput};
      if (input.empty()) {
        // user hit enter on an empty line
        continue;
      } else if (input.compare(0, 5, ".quit") == 0 || input.compare(0, 5, ".exit") == 0) {
        // exit the repl
        rx.history_add(input);
        break;
      }

      rx.history_add(input);

      // split string by space
      std::istringstream iss(input);
      std::vector<std::string> result{std::istream_iterator<std::string>(iss), {}};

      if (result[0] == "begin" || result[0] == "end") {
        if (result.size() == 2) {
          controller.add(result[1], result[0]);
        } else {
          spdlog::error("Wrong number of argument to begin/end command");
        }
        continue;
      }

      if (result[0] == "list") {
        auto tasks = controller.list(5);
        for (auto & i : tasks) {
          fmt::print("{: <30} {: <10} {}\n", std::get<0>(i), std::get<1>(i), std::get<2>(i));
        }
        continue;
      }

      spdlog::error("Unknown command: {}", result[0]);
    }

    rx.history_save(history_file);
    spdlog::info("Exiting Time Tracker");
  };
};
