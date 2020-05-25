
#pragma once

#include <date/date.h>
#include <fmt/format.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

class Controller {
public:
  Controller(std::string filename = "time-tracker.csv")
      : filename(filename) {
  }

  void add(const std::string &task, const std::string &status) {
    std::ofstream outfile;
    outfile.open(this->filename, std::ios_base::app); // append instead of overwrite
    outfile << lineFormatter(std::chrono::system_clock::now(), status, task);
    outfile.close();
  }

  std::vector<std::string> taskList(const std::string &search) const {
    std::ifstream infile;
    std::string line;
    const char delim = ';';

    std::set<std::string> tasks;
    infile.open(this->filename, std::ios_base::in);
    while (getline(infile, line)) {
      std::stringstream ss(line);

      std::string trash;
      std::getline(ss, trash, delim); // discard date
      std::getline(ss, trash, delim); // discard marker

      std::string task;
      std::getline(ss, task, delim);
      std::size_t found = task.rfind(search, 0);
      if (found == 0 || search.empty()) {
        tasks.insert(task);
      }
    }
    infile.close();

    return std::vector<std::string>(tasks.begin(), tasks.end());
  }

private:
  std::string lineFormatter(const std::chrono::time_point<std::chrono::system_clock> &time_point, const std::string &tag, const std::string &task) {
    std::string date = dateFormatter(time_point);
    return fmt::format("{0};{1};{2};\n", date, tag, task);
  }

  std::string dateFormatter(const std::chrono::time_point<std::chrono::system_clock> &time_point) {
    using namespace date;
    auto daypoint = floor<days>(time_point);
    auto ymd = year_month_day(daypoint);         // calendar date
    auto tod = make_time(time_point - daypoint); // Yields time_of_day type

    // Obtain individual components as integers
    auto y = int(ymd.year());
    auto m = unsigned(ymd.month());
    auto d = unsigned(ymd.day());
    auto h = tod.hours().count();
    auto min = tod.minutes().count();
    auto s = tod.seconds().count();

    return fmt::format("{0}-{1:02}-{2:02} {3:02}:{4:02}:{5:02}", y, m, d, h, min, s);
  }

  std::string filename;
};
