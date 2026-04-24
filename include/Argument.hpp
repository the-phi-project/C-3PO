// The Phi Project
// Copyright 2026 C-3PO Authors
// Use of this source code is governed by the GPL-3.0 license, found in the LICENSE file
//
// Date created: 22-04-2026
// Original author: Zevi Berlin (zeviberlin@gmail.com)
// Latest editor: Zevi Berlin (zeviberlin@gmail.com)

#ifndef C3PO_ARGUMENT_HPP
#define C3PO_ARGUMENT_HPP

#include <string>
#include <any>
#include <typeinfo>

//---------> [ Config. Separator ] <---------\\ 

namespace C3PO {

// Serves only for the purposes of inheritance to solve
// MAJOR MAJOR type system issues
struct ArgumentBase {
    std::string full;  // e.g. `--this-argument`, `--mode`
    std::string flag;  // e.g. `-ta`, `-m`

    // T{} value;

    std::string description;

    bool required;
    bool filled = false;

    //=====[ Declaration Separator ]=====\\ 

    ArgumentBase(const std::string& full_, const std::string& flag_,
                 const std::string& description_, bool required_)
        : full(full_), flag(flag_), description(description_), required(required_) {};

    virtual ~ArgumentBase() = default;

    virtual std::any get_value() const = 0;
    virtual const std::type_info& get_type() const = 0;  // so that the std::any can be cast
};

//================={ Header Item Separator }=================\\ 

// This is the real struct to use, the only issue is that
// it's a template so there are tons of issues with combining
// them into containers
template <typename TypeToParse>
struct Argument : ArgumentBase {
    TypeToParse value{};

    //=====[ Declaration Separator ]=====\\ 

    Argument(const std::string& full_, const std::string& flag_, const std::string& description_,
             bool required_)
        : ArgumentBase(full_, flag_, description_, required_) {
    }

    /**/

    void set(const TypeToParse& new_value) {
      this->value = new_value;
      this->filled = true;
    }

    TypeToParse get() const {
      return this->value;
    }

    /**/

    std::any get_value() const override {
      return std::any(this->value);
    }

    const std::type_info& get_type() const override {
      return typeid(TypeToParse);  // also valid with this->value as argument
    }
};

}  // namespace C3PO

#endif /* C3PO_ARGUMENT_HPP */