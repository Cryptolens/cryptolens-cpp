#pragma once

#include "basic_Error.hpp"

namespace serialkeymanager_com {

class Error : public basic_Error {
  // TODO: Why is it neccessary to prefix basic_Error here for the compiler to
  //       find the methods?
  int get_subsystem() const noexcept { return basic_Error::get_subsystem(api::main()); }
  int get_reason() const noexcept { return basic_Error::get_reason(api::main()); }
  size_t get_extra() const noexcept { return basic_Error::get_extra(api::main()); }
};

} // namespace serialkeymanager_com
