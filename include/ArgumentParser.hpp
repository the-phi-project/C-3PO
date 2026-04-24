// The Phi Project
// Copyright 2026 C-3PO Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 23-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef C3PO_ARGUMENT_PARSER_HPP
#define C3PO_ARGUMENT_PARSER_HPP

#include <string>
#include <any>
#include <typeinfo>
#include <vector>
#include <memory>

// clang-format off
#ifdef C3PO_NO_EXCEPTIONS
  #ifndef C3PO_NO_PRINT_ERRORS
    #include <iostream>
  #endif
#else
  #include <exception>
#endif
// clang-format on

#include "Argument.hpp"

//---------> [ Config. Separator ] <---------\\ 

namespace C3PO {

class ArgumentParser {
  private:
    // Store ArgumentBase instead of Argument to prevent a bunch of
    //  super annoying templating and typing issues
    // Use shared_ptr to prevent memory slicing which removes the
    //  value from the type, which is the only thing we really want
    std::vector<std::shared_ptr<ArgumentBase>> arguments;

    std::string help_menu;

  public:
    explicit ArgumentParser(const std::string& custom_help_menu = "")
        : help_menu(custom_help_menu) {
    }

    ~ArgumentParser() = default;
    ArgumentParser(const ArgumentParser& other) = delete;  // no-copy resource

    //=====[ Declaration Separator ]=====\\ 

    template <typename TypeToParse>
    constexpr void createArgument(const std::string& full, const std::string& flag,
                                  const std::string& description, bool required) {
      this->arguments.push_back(
        std::make_shared<Argument<TypeToParse>>(full, flag, description, required));
    }
};

}  // namespace C3PO

#endif /* C3PO_ARGUMENT_PARSER_HPP */