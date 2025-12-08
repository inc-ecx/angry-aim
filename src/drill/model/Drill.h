#ifndef DRILL_H
#define DRILL_H

#include <string>
#include "inc/inc_json.h"

class Drill {
public:
  std::string package;
  std::string name;
  std::string title;
  std::string description;
  std::string controllerPackage;
  std::string controllerName;
  json controllerConfig;

  Drill(
    const std::string &package, const std::string &name, const std::string &title, const std::string &description,
    const std::string &controller_package, const std::string &controller_name, const json &controller_config
  ) :
    package(package),
    name(name),
    title(title),
    description(description),
    controllerPackage(controller_package),
    controllerName(controller_name),
    controllerConfig(controller_config) {}
};

#endif //DRILL_H
