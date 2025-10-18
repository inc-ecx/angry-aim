#ifndef DRILL_H
#define DRILL_H

#include <string>

class Drill {
public:
  std::string name;
  std::string description;
  std::string link;

  Drill(const std::string &name, const std::string &description, const std::string &link)
    : name(name),
      description(description),
      link(link) {
  }
};

#endif //DRILL_H
