
#include <verbList.hpp>
#include <verbRepl.hpp>
#include <verbStatus.hpp>
#include <verbTaskCompletion.hpp>

#include <cxxsubs.hpp>
#include <cxxopts.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>

#include <fstream>


int main(int argc, char *argv[]) {
  try {
    spdlog::cfg::load_env_levels();

    cxxsubs::Verbs<cxxsubs::CompletionCommand, OptionsStatus<Begin>, OptionsStatus<End>, OptionsRepl, OptionsList, OptionsTaskCompletion>(argc, argv);

  } catch (const cxxopts::OptionException &e) {
    spdlog::error("Parsing options : {}", e.what());
    return 1;
  } catch (const std::exception &e) {
    spdlog::error("{}", e.what());
    return 1;
  }
  return 0;

}
