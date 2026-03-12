#pragma once

#include "causis/Binding.h"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace causis {

class Environment {
public:
  explicit Environment(Environment *parent = nullptr) : _parent(parent) {}

  void define(const std::string &name, Binding binding) {
    _values[name] = std::move(binding);
  }

  Binding &get(const std::string &name) {
    auto found = _values.find(name);
    if (found != _values.end()) {
      return found->second;
    }

    if (_parent != nullptr) {
      return _parent->get(name);
    }

    throw std::runtime_error("Unknown binding: " + name);
  }

  void assign(const std::string &name, const Value &value) {
    Binding &binding = get(name);
    binding.value = value;
  }

private:
  std::unordered_map<std::string, Binding> _values;
  Environment *_parent = nullptr;
};

} // namespace causis
