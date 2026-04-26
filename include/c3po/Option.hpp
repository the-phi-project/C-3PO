// The Phi Project
// Copyright 2026 C-3PO Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 24-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef C3PO_OPTION_HPP
#define C3PO_OPTION_HPP

#include <string>
#include <exception>
#include <type_traits>

//---------> [ Config. Separator ] <---------\\ 

namespace C3PO {

/// Wrapper class for just a single command line argument
class Option {
  private:
    std::string full;
    std::string flag;
    std::string description;

    std::string value;

    bool takes_value;
    bool required;
    bool filled = false;

  public:
    Option(const std::string& full_, const std::string& flag_, const std::string& description_,
           bool takes_value_, bool required_)
        : full(full_),
          flag(flag_),
          description(description_),
          takes_value(takes_value_),
          required(required_) {
    }

    explicit Option(const std::string& flag_, bool takes_value_)
        : flag(flag_), takes_value(takes_value_), required(false) {
    }

    Option(const Option& opt) = delete;  // too expensive to copy

    ~Option() = default;

    //=====[ Declaration Separator ]=====\\ 

    // clang-format off
    /** GETTERS **/
    std::string getFull() const { return this->full; }
    std::string getFlag() const { return this->flag; }
    std::string getDescription() const { return this->description; }

    std::string getValueAsString() const { return this->value; }

    bool getTakesValue() const { return this->takes_value; }
    bool getRequired() const { return this->required; }
    bool getFilled() const { return this->filled; }

    /** SETTERS **/
    void setValue(const std::string &value_) { this->value = value_; this->filled = true;}

    void setFilled(bool filled_) { this->filled = filled_; }

    // clang-format on

    /// Retrieve the value to a given type, check docs for
    /// the exact methods it uses to parse a string into the types
    /// @returns typed value or nullptr (if no exceptions)
    /// @note if C3PO_NO_EXCEPTIONS is NOT defined, it will throw on conversion failure
    /// @throws std::runtime_error
    template <typename TypeToReturn>
    constexpr TypeToReturn valueAs() const {
      if (std::is_same<TypeToReturn, bool>::value && !this->takes_value) return this->filled;

      if constexpr (std::is_same<TypeToReturn, std::string>::value) return this->value;

      /**/

      if constexpr (std::is_same<TypeToReturn, bool>::value) {
        if (this->value.empty()) return false;

        const char first = this->value[0];
        if (first == 'T' || first == 't' || first == '1') return true;
        if (first == 'F' || first == 'f' || first == '0') return false;

#ifdef C3PO_NO_EXCEPTIONS
        return nullptr;
#else
        throw std::runtime_error("Unable to cast `" + this->value + "` to `bool` due to ambiguity");
#endif
      }

      /**/

      if constexpr (std::is_same<TypeToReturn, char>::value)
        return (this->value.empty() ? 0 : this->value[0]);

      /**/

      if constexpr (std::is_arithmetic<TypeToReturn>::value) {
        try {
          if (std::is_floating_point<TypeToReturn>::value) {
            return (TypeToReturn)(std::stod(this->value));
          }

          // put the number into the biggest container, then shrink if necessary
          return (TypeToReturn)(std::stoll(this->value));
        } catch (const std::invalid_argument& exc) {
#ifdef C3PO_NO_EXPCETIONS
          return nullptr;
#else
          throw std::runtime_error("Unable to convert `" + this->value + "` to a number");
#endif
        } catch (const std::out_of_range& exc) {
#ifdef C3PO_NO_EXCEPTIONS
          return nullptr;
#else
          throw std::runtime_error("The number `" + this->value + "` is too large to hold");
#endif
        }
      }
    }
};

}  // namespace C3PO

#endif /* C3PO_OPTION_HPP */