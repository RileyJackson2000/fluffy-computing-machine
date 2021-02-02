#pragma once

#include <string>

namespace fcm {

class Scene {
  // std string for testing purposes
  std::string _name;
public:
  Scene(std::string name): _name{std::move(name)} {}
  const std::string &name() const {return _name;}
};

}
