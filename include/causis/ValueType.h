/**
 * @file ValueType.h
 * @author Amin Karic
 * @brief Runtime value kind enumeration for Causis.
 */
#pragma once

namespace causis {

/**
 * @brief Enumerates the runtime value kinds supported by the interpreter.
 */
enum class ValueType {
  Int,
  String,
  Bool,
  Function,
  Void
};

} // namespace causis
