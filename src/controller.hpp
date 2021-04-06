
#pragma once

#include <date/date.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <fstream>
#include <set>
#include <string>
#include <tuple>
#include <filesystem>
#include <iostream>


class Controller {
  using json = nlohmann::json;
public:
  Controller(std::string filename = "timetracker.json")
      : filename(filename) {
    if (!std::filesystem::exists(this->filename)) {
      std::ofstream outfile;
      outfile.open(this->filename, std::ios_base::app); // append instead of overwrite
      outfile << "[\n";
      outfile.close();
    }

  }

  void add(const std::string &task, const std::string &status) {
    std::ofstream outfile;
    outfile.open(this->filename, std::ios_base::app); // append instead of overwrite
    outfile << lineFormatter(std::chrono::system_clock::now(), status, task) << "\n,\n";
    outfile.close();
  }

  std::vector<std::tuple<std::string, std::string, std::string>> list(int limit) const {

    std::ifstream infile;
    std::string line;
    std::vector<std::tuple<std::string, std::string, std::string>> tasks;

    infile.open(this->filename, std::ios_base::in);
    while (getline(infile, line)) {
      std::stringstream ss(line);

      if (line == "[") {
        // escape the first line that contain only one [
        continue;
      }
      if (line == ",") {
        // escape intermediate line that contain only one ,
        continue;
      }

      json j = json::parse(line);
      tasks.push_back({j["date_full"], j["ph"], j["name"]});
    }
    infile.close();

    return std::vector<std::tuple<std::string, std::string, std::string>>(tasks.end()-limit, tasks.end());;
  }

  std::vector<std::string> taskList(const std::string &search) const {

    std::ifstream infile;
    std::string line;
    std::set<std::string> tasks;

    infile.open(this->filename, std::ios_base::in);
    while (getline(infile, line)) {
      std::stringstream ss(line);

      if (line == "[") {
        // escape the first line that contain only one [
        continue;
      }
      if (line == ",") {
        // escape intermediate line that contain only one ,
        continue;
      }

      json j = json::parse(line);
      const std::string taskName = j["name"].get<std::string>();

      std::size_t found = taskName.rfind(search, 0);
      if (found == 0 || search.empty()) {
        tasks.insert(taskName);
      }
    }
    infile.close();

    return std::vector<std::string>(tasks.begin(), tasks.end());
  }

private:
  std::string lineFormatter(const std::chrono::time_point<std::chrono::system_clock> &time_point, const std::string &status, const std::string &task) {
    json j = {
      {"name", task},
      {"cat", "PERF"},
      {"ph", status == "begin" ? "B" : "E"},
      {"pid", pidFormatter(time_point)},
      {"tid", task},
      {"ts", std::chrono::duration_cast<std::chrono::minutes>(time_point.time_since_epoch()).count()},
      {"date_full", dateFormatter(time_point)}
    };
    return  j.dump();
  }

  static std::string dateFormatter(const std::chrono::time_point<std::chrono::system_clock> &time_point) {
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

  //! The pid represent the day of the years
  static int pidFormatter(const std::chrono::time_point<std::chrono::system_clock> &time_point) {
    using namespace date;

    const auto daypoint = floor<days>(time_point);
    const auto ymd = year_month_day{daypoint};

    // calculating the year and the day of the year
    const auto year = ymd.year();
    const auto year_day = daypoint - sys_days{year/January/0};
    return year_day.count();
  }

  std::string filename;
};
