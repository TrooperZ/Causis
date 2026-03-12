#pragma once

#include "causis/Binding.h"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace causis {

class Environment {
public:
  explicit Environment(Environment *parent = nullptr) : parent_(parent) {}

  void define(const std::string &name, Binding binding) {
    values_[name] = std::move(binding);
  }

  Binding &get(const std::string &name) {
    auto found = values_.find(name);
    if (found != values_.end()) {
      return found->second;
    }

    if (parent_ != nullptr) {
      return parent_->get(name);
    }

    throw std::runtime_error("Unknown binding: " + name);
  }

  void assign(const std::string &name, const Value &value) {
    Binding &binding = get(name);
    binding.value = value;
  }

private:
  std::unordered_map<std::string, Binding> values_;
  Environment *parent_ = nullptr;
};

} // namespace causis
