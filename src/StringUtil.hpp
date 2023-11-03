//
// Created by caracal on 11/1/23.
//

#ifndef PASSWORDMANAGERQT6_STRINGUTIL_HPP
#define PASSWORDMANAGERQT6_STRINGUTIL_HPP


class StringUtil {
public:
  static std::string sha256(std::string input);

  static std::string sha3_512(std::string input);

  static std::string aes_encrypt(const std::string &input, const std::string &key);

  static std::string aes_decrypt(const std::string &input, const std::string &key);

  static std::string generate_salt();
};


#endif //PASSWORDMANAGERQT6_STRINGUTIL_HPP
