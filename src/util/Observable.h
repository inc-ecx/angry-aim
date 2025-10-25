#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>
#include <functional>
#include <memory>

class Observation {
  std::function<void()> cleanup;

public:
  explicit Observation(std::function<void()>&& cleanup) : cleanup(cleanup) {
  }

  ~Observation() {
    if (cleanup) cleanup();
  }

  Observation(const Observation &other) = delete;

  Observation(Observation &&other) noexcept :
    cleanup(std::move(other.cleanup)) {}

  Observation & operator=(const Observation &other) = delete;

  Observation & operator=(Observation &&other) noexcept {
    if (this == &other)
      return *this;
    cleanup = std::move(other.cleanup);
    return *this;
  }
};

class Observable : public std::enable_shared_from_this<Observable> {
  class Token {};

  std::vector<std::shared_ptr<std::function<void()>>> _listeners;

public:
  explicit Observable(Token) {
  }

  static std::shared_ptr<Observable> make() {
    return std::make_shared<Observable>(Token{});
  }

  void trigger() {
    for (auto &listener : _listeners) listener->operator()();
  }

  std::shared_ptr<Observation> listen(std::function<void()> listener) {
    auto me = shared_from_this();
    auto ptr = std::make_shared<std::function<void()>>(listener);
    _listeners.push_back(ptr);
    return std::make_shared<Observation>([me, ptr]() {
      auto it = std::ranges::find(me->_listeners, ptr);
      if (it != me->_listeners.end()) me->_listeners.erase(it);
    });
  }
};

#endif //OBSERVABLE_H
