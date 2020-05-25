#pragma once

#include <cxxsubs.hpp>
#include <replxx.hxx>

#include <fstream>

using Replxx = replxx::Replxx;

class OptionsRepl : public cxxsubs::IOptions {
public:
  OptionsRepl()
      : cxxsubs::IOptions({"repl"}, "Interactive mode") {
    this->options.add_options()
      ("help", "Print help")
      ;
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help() << std::endl;
      exit(0);
    }
  }

  void exec() {
	  Replxx rx;

    std::string history_file {"./time-tracker-repl-history.txt"};
    rx.history_load(history_file);

	  std::string prompt {"\x1b[1;32mtimetracker\x1b[0m> "};

    while (true) {
      char const* cinput{ nullptr };
      do {
        cinput = rx.input(prompt);
      } while ( ( cinput == nullptr ) && ( errno == EAGAIN ) );

      if (cinput == nullptr) {
        break;
      }
    }

	  rx.history_save(history_file);
  };
};
