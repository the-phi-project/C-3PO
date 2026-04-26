// The Phi Project
// Copyright 2026 C-3PO Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 25-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef C3PO_OPTION_PARSER_HPP
#define C3PO_OPTION_PARSER_HPP

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <type_traits>

#include "c3po/Option.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace C3PO {

class OptionParser {
  private:
    std::vector<std::shared_ptr<Option>> options{};

    std::string help_menu_header;

  public:
    OptionParser(const std::string& custom_header = "A Program") : help_menu_header(custom_header) {
    }

    OptionParser(const OptionParser& optp) = delete;  // too expensive to copy

    ~OptionParser() = default;

//=====[ Declaration Separator ]=====\\ 

#ifndef C3PO_NO_EXCEPTIONS
    bool
#else
    void
#endif
    parse(int argc, char** argv) {
    }

    /**/

    /// Create a new option that should be parsed
    /// @param full the full name, e.g. '--some-option'
    /// @param flag the short flag, e.g. '-s' or '-so'
    /// @param description a blurb about what the option represents
    /// @param takes_value should the option accept a value or just be present?
    /// @param required does this flag need to be present under all circumstances?
    /// @returns the std::shared_ptr to the Option created
    std::shared_ptr<Option> defineOption(const std::string& full, const std::string& flag,
                                         const std::string& description, bool takes_value,
                                         bool required) {
      std::shared_ptr<Option> opt =
        std::make_shared<Option>(full, flag, description, takes_value, required);
      this->options.push_back(opt);
      return opt;
    }

    /// Retrieve an option from the defined options
    /// @param full_or_flag the full name or flag of the option to get
    /// @returns std::shared_ptr of the Option or nullptr
    std::shared_ptr<Option> getOption(const std::string& full_or_flag) {
      for (const auto& opt : this->options) {
        if (opt->getFull() == full_or_flag || opt->getFlag() == full_or_flag) return opt;
      }

      return nullptr;
    }
};

}  // namespace C3PO

#endif /* C3PO_OPTION_PARSER_HPP */