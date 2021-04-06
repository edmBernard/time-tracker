#pragma once

#include <controller.hpp>

#include <cxxsubs.hpp>
#include <spdlog/spdlog.h>

#include <fstream>


class OptionsTaskCompletion : public cxxsubs::IOptions {
public:
  OptionsTaskCompletion()
      : cxxsubs::IOptions({"task_completion"}, "Task completion command") {
    this->options.add_options()
      ("help", "Print help")
      ("show", "show completion code to add in bashrc", cxxopts::value<bool>())
      ("exec_name", "the executable name to add in completion script", cxxopts::value<std::string>())
      ("s,search", "String to search in task name", cxxopts::value<std::string>()->default_value(""), "SEARCH");
    this->options.parse_positional({"search"});
    this->options.positional_help("[<search>]");
  }

  void validate() {
    if (this->parse_result->count("help")) {
      std::cout << this->options.help() << std::endl;
      exit(0);
    }

    if (this->parse_result->count("show") ^ this->parse_result->count("exec_name")) {
      spdlog::error("Parsing options: 'show' and 'exec_name' must be filled");
      exit(1);
    }
  }

  void exec() {
    if (this->parse_result->count("show")) {
      std::string my_name = (*this->parse_result)["exec_name"].as<std::string>();
      std::cout << "_" + my_name +"_completions() {\n"
                  "  local cur_word args type_list\n"
                  "  cur_word=\"${COMP_WORDS[COMP_CWORD]}\"\n"
                  "  args=(\"${COMP_WORDS[@]}\")\n\n"
                  "  type_list=\"$(" + my_name + " completion ${args[1]}) $(" + my_name + " task_completion ${args[@]:2})\"\n"
                  "  COMPREPLY=( $(compgen -W \"${type_list}\" -- ${cur_word}) )\n\n"
                  "  # if no match was found, fall back to filename completion\n"
                  "  if [ ${#COMPREPLY[@]} -eq 0 ]; then\n"
                  "    COMPREPLY=( $(compgen -f -- \"${cur_word}\" ) )\n"
                  "  fi\n"
                  "  return 0\n"
                  "}\n"
                  "complete -F _" + my_name + "_completions " + my_name
                << std::endl;
    } else {
      auto task = (*this->parse_result)["search"].as<std::string>();
      Controller controller;
      auto tasks = controller.taskList(task);

      for (auto i : tasks) {
        std::cout << i << std::endl;
      }
    }
  };
};
