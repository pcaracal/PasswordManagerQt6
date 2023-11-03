//
// Created by caracal on 11/1/23.
//

#include <string>
#include <fstream>
#include "FileUtil.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::string FileUtil::read_file(const std::string &path) {
  std::ifstream f(path);
  if (f.is_open()) {
    std::string content((std::istreambuf_iterator<char>(f)),
                        (std::istreambuf_iterator<char>()));
    f.close();
    return content;
  }
  return "-1";
}

void FileUtil::write_file(const std::string &path, const std::string &content) {
  std::ofstream of(path);
  of << content;
  of.close();
}

json FileUtil::read_json(const std::string &path) {
  std::ifstream f(path);
  json j;
  f >> j;
  return j;
}

void FileUtil::write_json(const std::string &path, const json &content) {
  std::ofstream f(path);
  std::string s = content.dump(2);
  f << s;
  f.close();
}