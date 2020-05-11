
#include <verbShow.hpp>
#include <verbStatus.hpp>

#include <cxxsubs.hpp>
#include <cxxopts.hpp>

#include <fstream>


int main(int argc, char *argv[]) {
  try {
    cxxsubs::Verbs<cxxsubs::CompletionCommand, OptionsStatus<Begin>, OptionsStatus<End>>(argc, argv);

  } catch (const cxxopts::OptionException &e) {
    std::cout << "Error: parsing options: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
