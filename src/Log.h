#ifndef LOG_H
#define LOG_H

#include <string>
#include <iostream>

class Log {
public:
  static void info(std::string&& msg) {
    std::cout << "[inf] " << msg << std::endl;
  }
  static void warn(std::string&& msg) {
    std::cout << "[wrn] " << msg << std::endl;
  }
  static void error(std::string&& msg) {
    std::cout << "[ERR] " << msg << std::endl;
  }
  static void fatal(std::string&& msg) {
    std::cout << "[FATAL] " << msg << std::endl;
  }
};

#endif //LOG_H
