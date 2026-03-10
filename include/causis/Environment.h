/**
 * @file Environment.h
 * @author Amin Karic
 * @brief Runtime lexical scope representation for Causis.
 *
 * @details
 * An environment stores named bindings and can reference a parent environment
 * to support lexical lookup across nested scopes.
 */
#pragma once

#include "causis/Binding.h"

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace causis {

/**
 * @brief Represents one runtime scope of bindings.
 */
class Environment {
public:
  /**
   * @brief Construct an environment with an optional parent scope.
   *
   * @param parent Parent environment used for outer-scope lookup.
   */
  explicit Environment(Environment *parent = nullptr) : parent_(parent) {}

  /**
   * @brief Define a new binding in the current scope.
   *
   * @param name Binding name.
   * @param binding Binding data to store.
   */
  void define(const std::string &name, Binding binding) {
    values_[name] = std::move(binding);
  }

  /**
   * @brief Resolve a binding by name, searching parent scopes as needed.
   *
   * @param name Binding name.
   * @return Binding& Mutable reference to the resolved binding.
   *
   * @throws std::runtime_error If the binding does not exist.
   */
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

  /**
   * @brief Assign a new runtime value to an existing binding.
   *
   * @param name Binding name.
   * @param value New runtime value.
   */
  void assign(const std::string &name, const Value &value) {
    Binding &binding = get(name);
    binding.value = value;
  }

private:
  std::unordered_map<std::string, Binding> values_;
  Environment *parent_ = nullptr;
};

} // namespace causis
