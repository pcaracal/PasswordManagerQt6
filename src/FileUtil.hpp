//
// Created by caracal on 11/1/23.
//

#ifndef PASSWORDMANAGERQT6_FILEUTIL_HPP
#define PASSWORDMANAGERQT6_FILEUTIL_HPP

#include <nlohmann/json.hpp>

class FileUtil {
public:
  static std::string read_file(const std::string &path);

  static void write_file(const std::string &path, const std::string &content);

  static nlohmann::json read_json(const std::string &path);

  static void write_json(const std::string &path, const nlohmann::json &content);

  static void create_file(const std::string &path);
};


#endif //PASSWORDMANAGERQT6_FILEUTIL_HPP
