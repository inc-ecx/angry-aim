#ifndef TOGGELABLE_H
#define TOGGELABLE_H

class Toggelable {
private:
  bool enabled;

public:
  explicit Toggelable(bool enabled) : enabled(enabled) {
  }

  virtual ~Toggelable() = default;

  virtual void onEnable() {
  }

  virtual void onDisable() {
  }

  void setEnabled(bool enabled) { this->enabled = enabled; }
  bool isEnabled() { return enabled; }
};

#endif //TOGGELABLE_H
